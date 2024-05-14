/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sessiz <sessiz@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:51:28 by mitasci           #+#    #+#             */
/*   Updated: 2024/05/14 15:22:36 by sessiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	pthread_mutex_lock(&fork2->lock);
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
}

void	ft_eat(t_philo *philo)
{
	if (philo->table->philo_die != 1)
	{
		//pthread_mutex_lock(&philo->table->deadlock);
		ft_take_fork(philo, philo->table);
		pthread_mutex_lock(&philo->lock);
		write_message(ft_itoa(get_time() - philo->table->start_time), ft_itoa(philo->id), "is eating\n");
		ft_usleep(philo->table->time_to_eat);
		philo->eat_count++;
		philo->last_eat = get_time();
		philo->eating = 1;
		pthread_mutex_unlock(&philo->lock);
		//pthread_mutex_unlock(&philo->table->deadlock);
		ft_leave_fork(philo, philo->table);
		ft_sleep(philo, *philo->table);
	}
}

void	ft_think(t_philo *philo, t_table table)
{
	write_message(ft_itoa(get_time() - table.start_time), ft_itoa(philo->id), "is thinking\n");
	ft_usleep(table.time_to_eat);
}

void	ft_sleep(t_philo *philo, t_table table)
{
	//pthread_mutex_lock(&philo->table->deadlock);
	write_message(ft_itoa(get_time() - table.start_time), ft_itoa(philo->id), "is sleeping\n");
	ft_usleep(table.time_to_sleep);
	//pthread_mutex_unlock(&philo->table->deadlock);
}

void	*live(void *arg)
{
	t_philo *philo;
	philo = (t_philo *)arg;
	while (1)
	{
		//pthread_mutex_lock(&philo->table->deadlock);
		int lasteat = get_time() - philo->last_eat;
		if (lasteat >= philo->table->time_to_die)
		{
			write_message(ft_itoa(get_time() - philo->table->start_time), ft_itoa(philo->id), "died\n"); //eğer ölen threadin die 1 ise threadler destroy edilmeli
			ft_dead(philo);
			//philo->table->philo_die = 1;
			//pthread_mutex_unlock(&philo->table->deadlock);
		}
		//pthread_mutex_unlock(&philo->table->deadlock);
		if(philo->eat_count == 0)
			ft_eat(philo);
		else
			break;
	}
	philo->eating = 0;
	return (arg);
}


void ft_dead(t_philo *philo)
{
	int i;

	i=0;
	while (i < philo->table->philo_no)
	{
		pthread_join((philo->table->philos[i]).th, NULL);
		i++;
	}
	i = 0;
	while (i < philo->table->philo_no)
	{
		pthread_mutex_destroy(&philo->table->forks[i].lock);
		pthread_mutex_destroy(&philo->table->philos[i].lock);
		i++;
	}
	//pthread_mutex_destroy(&philo->table->deadlock);
	free(philo->table->philos);
	free(philo->table->forks);
	exit(0);
}