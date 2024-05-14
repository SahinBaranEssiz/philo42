/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sessiz <sessiz@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 14:41:46 by mitasci           #+#    #+#             */
/*   Updated: 2024/05/09 23:32:53 by sessiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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