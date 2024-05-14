#include "philo.h"

static size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i += 1;
	return (i);
}

static int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	c1;
	unsigned char	c2;
	size_t			i;

	i = 0;
	while (i < n)
	{
		c1 = (unsigned char)s1[i];
		c2 = (unsigned char)s2[i];
		if (c1 != c2)
		{
			if (c1 > c2)
				return (1);
			else
				return (-1);
		}
		i += 1;
		if (c1 == 0 && c2 == 0)
			break ;
	}
	return (0);
}

static int	ft_isdigit(int c)
{
	return (c >= 48 && c <= 57);
}

static int	int_in_limit(const char *s)
{
	size_t	len;

	len = ft_strlen(s);
	if ((len > 10 && s[0] != '-') || (s[0] == '-' && len > 11))
		return (0);
	if (len == 10 && s[0] != '-'
		&& ft_strncmp(s, "2147483647", 10) > 0)
		return (0);
	if (len == 11 && s[0] == '-'
		&& ft_strncmp(s, "-2147483648", 11) > 0)
		return (0);
	return (1);
}

int	valid_int(const char *s)
{
	int	i;

	if (!int_in_limit(s))
		return (0);
	i = 0;
	if (s[0] == '-' || s[0] == '+')
		i++;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

void	write_message(char *time,char *id, char* act)
{
	// write(1, time, ft_strlen(time));
	// write(1, " ", 1);
	// write(1, id, ft_strlen(id));
	// write(1, " ", 1);
	// write(1, act, ft_strlen(act));

	printf("%s %s %s",time,id,act);
}

u_int64_t	get_time(void)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL))
		return (-1);
	return (tv.tv_sec * (u_int64_t)1000 + tv.tv_usec / (u_int64_t)1000);
}

void	ft_usleep(int milliseconds)
{
	usleep(milliseconds * 1000);
}

void	ft_take_fork(t_philo *philo, t_table *table)
{
	t_fork	*fork1;
	t_fork	*fork2;

	fork1 = &(table->forks[philo->id - 1]);
	if (philo->id == 1)
		fork2 = &(table->forks[table->philo_no - 1]);
	else
		fork2 = &(table->forks[philo->id - 2]);
	pthread_mutex_lock(&fork1->lock);
	//printf("fork id:%d philo id:%d has taken a fork\n", fork1->id, philo->id);
	pthread_mutex_lock(&fork2->lock);
	//printf("fork id:%d philo id:%d has taken a fork\n", fork2->id, philo->id);
}

void	ft_leave_fork(t_philo *philo, t_table *table)
{
	t_fork	*fork1;
	t_fork	*fork2;

	fork1 = &(table->forks[philo->id - 1]);
	if (philo->id == 1)
		fork2 = &(table->forks[table->philo_no - 1]);
	else
		fork2 = &(table->forks[philo->id - 2]);

	pthread_mutex_unlock(&fork1->lock);
	pthread_mutex_unlock(&fork2->lock);
	ft_sleep(philo, *table);
}

void	ft_eat(t_philo *philo)
{
	ft_take_fork(philo, philo->table);
	pthread_mutex_lock(&philo->lock);
	write_message(ft_itoa(get_time() - philo->table->start_time), ft_itoa(philo->id), "is eating\n");
	ft_usleep(philo->table->time_to_eat);
	pthread_mutex_unlock(&philo->lock);
	ft_leave_fork(philo, philo->table);
}

void	ft_think(t_philo *philo, t_table table)
{
	write_message(ft_itoa(get_time() - table.start_time), ft_itoa(philo->id), "is thinking\n");
	ft_usleep(table.time_to_eat);
}

void	ft_sleep(t_philo *philo, t_table table)
{
	write_message(ft_itoa(get_time() - table.start_time), ft_itoa(philo->id), "is sleeping\n");
	ft_usleep(table.time_to_sleep);
}

void	*live(void *arg)
{
	t_philo *philo;
	philo = (t_philo *)arg;
	//printf("%s, %d living\n",ft_itoa(get_time() - philo->table->start_time) , philo->id);
	ft_eat(philo);
	return (arg);
}

static int	args_valid(int argc, char **argv)
{
	int	i;
	
	if (argc != 5 && argc != 6)
		return (0);
	i = 1;
	while (i < argc - 1)
	{
		if (!valid_int(argv[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	philo_init(t_philo *philo, t_table *table, int id)
{
	philo->id = id;
	philo->dead = 0;
	philo->eating = 1;
	philo->sleeping = 0;
	philo->thinking = 0;
	philo->timestamp = 0;
	philo->table = table;
	//printf("%s ,%d th created\n",ft_itoa(get_time() - table->start_time) , philo->id);
	pthread_mutex_init(&philo->lock, NULL);
	pthread_create(&(philo->th), NULL, live, philo);
}

static void	fork_init(t_fork *fork, int id)
{
	fork->id = id;
	pthread_mutex_init(&fork->lock, NULL);
}

static void	table_init(t_table *table, int argc, char **argv)
{
	int	i;

	table->philo_no = ft_atoi(argv[1]);
	table->philos = (t_philo *)malloc(sizeof(t_philo) * table->philo_no);
	table->forks = (t_fork *)malloc(sizeof(t_fork) * table->philo_no);
	if (!(table->philos))
		return ;
	table->start_time = get_time();
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->philo_eat_no = 0;
	if (argc == 6)
		table->philo_eat_no = ft_atoi(argv[5]);
	i = 0;
	while (i < table->philo_no)
	{
		philo_init(&(table->philos[i]), table, i + 1);
		fork_init(&(table->forks[i]), i + 1);
		i++;
	}
}


void	table_destroy(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_no)
	{
		pthread_join((table->philos[i]).th, NULL);
		i++;
	}
	free(table->philos);
	free(table->forks);
}

int	main(int argc, char **argv)
{
	t_table		table;
	
	if (!args_valid(argc, argv)) //argümanların 0dan büyük olduğunu kontrol et
		return (1);
	table_init(&table, argc, argv);

	table_destroy(&table);
}

static int	is_whitespace(int c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

static int	ft_isdig(int c)
{
	return (c >= 48 && c <= 57);
}

int	ft_atoi(const char *str)
{
	int	res;
	int	i;
	int	is_neg;

	res = 0;
	i = 0;
	is_neg = 1;
	while (is_whitespace(str[i]))
		i += 1;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			is_neg *= -1;
		i += 1;
	}
	while (str[i])
	{
		if (!ft_isdig(str[i]))
			break ;
		res *= 10;
		res += str[i] - '0';
		i += 1;
	}
	return (is_neg * res);
}

static size_t	char_count(int n)
{
	size_t	i;

	i = 1;
	if (n < 0)
		return (char_count(-n) + 1);
	while (n > 9)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static char	*handle_negmaxnum(int n)
{
	char	*s;
	size_t	len;

	len = 11;
	s = (char *)malloc(12);
	if (!s)
		return (NULL);
	s[0] = '-';
	s[1] = '2';
	while (n > 0)
	{
		s[len - 1] = n % 10 + '0';
		n /= 10;
		len--;
	}
	s[11] = 0;
	return (s);
}

char	*ft_itoa(int n)
{
	char	*str;
	size_t	len;
	size_t	org_len;

	if (n == -2147483648)
		return (handle_negmaxnum(147483648));
	len = char_count(n);
	org_len = len;
	str = (char *)malloc(len + 1);
	if (!str)
		return (NULL);
	if (n == 0)
		str[0] = '0';
	else if (n < 0)
	{
		str[0] = '-';
		n *= -1;
	}
	while (n > 0)
	{
		str[len-- - 1] = n % 10 + '0';
		n /= 10;
	}
	str[org_len] = 0;
	return (str);
}