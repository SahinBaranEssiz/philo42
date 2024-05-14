/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sessiz <sessiz@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 14:25:13 by mitasci           #+#    #+#             */
/*   Updated: 2024/05/10 03:40:53 by sessiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdint.h>

//argumanları alıp doğru sayıda geldiğini kontrol ettik x
// gelen arguman sayısıyla philo oluşturduk x
//her philo ile thread oluşturduk x
//bütün threadlerin bitmesini bekledik
//fork kontrolü yapılıyor x
// has taken fork yazısı doğru basılmalı program loopa giriyor
//philonun eat think ve sleep planlaması yapılmalı
//sıra eat - sleep - think şeklinde
// eat think sleep actlerini yaptık x
// philonun doğru zaman aralığında yemek yediği kontrol edilmeli 
// ölen bir philo olduğunda program  10 ms içersinde bitmeli

typedef struct s_table t_table;

typedef struct s_philo
{
	int			id;
	int			dead;
	int			eat_count;
	int			eating;
	int			thinking;
	pthread_t	th;
	int			last_eat;
	t_table		*table;
	pthread_mutex_t	lock;
}	t_philo;


typedef struct s_fork
{
	int				id;
	pthread_mutex_t	lock;
}	t_fork;

typedef struct s_table
{
	int		philo_no;
	t_philo	*philos;
	t_fork	*forks;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		philo_eat_no;
	int 	philo_die;
	u_int64_t	start_time;
	pthread_mutex_t	deadlock;
}	t_table;


//utils
int	valid_int(const char *s);
void	write_message(char *time,char *id, char* act);
u_int64_t	get_time(void);
void	ft_usleep(int milliseconds);

//actions
void	ft_eat(t_philo *philo);
void	ft_think(t_philo *philo, t_table table);
void	ft_sleep(t_philo *philo, t_table table);
void	*live(void *philo);
int	check_forks(t_philo philo, t_table table);
void ft_dead(t_philo *philo);

//libft
int	ft_atoi(const char *str);
char	*ft_itoa(int n);
void	table_destroy(t_table *table);

#endif