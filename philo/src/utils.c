/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 15:09:37 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/06/02 16:11:13 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

unsigned long	gtime(void)
{
	unsigned long	ms;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	ms = (unsigned long)(tv.tv_sec)*1000
		+ (unsigned long)(tv.tv_usec) / 1000;
	return (ms * 1000);
}

static void	clean(t_table *table)
{
	int	i;

	if (table->forks)
	{
		i = -1;
		while (++i < table->n)
			pthread_mutex_destroy(&(table->forks[i]));
		free(table->forks);
	}
	if (table->reapers)
	{
		i = -1;
		while (++i < table->n)
			pthread_mutex_destroy(&(table->reapers[i]));
		free(table->reapers);
	}
	if (table->philos)
	{
		i = -1;
		while (++i < table->n)
			free(table->philos[i].last_eat);
		free(table->philos);
	}
	pthread_mutex_destroy(&table->qmut);
	free(table);
}

int	exit_(int status, t_table *t)
{
	if (t)
		clean(t);
	if (status == 1)
		printf("Bad args\n");
	else if (status == 2)
		printf("Error on Malloc\n");
	else if (status == 3)
		printf("Error on mutex creation\n");
	else if (status == 4)
		printf("Error on pthread creation\n");
	else if (status == 5)
		printf("Error on pthread join\n");
	return (status);
}

t_philos	*init_philo(t_table *table)
{
	unsigned long	*time;
	t_philos		*phi;
	int				i;

	phi = malloc(sizeof(t_philos) * table->n);
	if (!phi)
		return (NULL);
	i = -1;
	while (++i < table->n)
	{
		time = malloc(sizeof(unsigned long));
		*time = table->s_time;
		phi[i] = (t_philos){i + 1, 0, time, 0, table};
	}
	return (phi);
}

int	i_lock(t_table *t, t_philos *phi, char fos)
{
	int	first;
	int	second;

	if (phi->n % 2)
	{
		first = phi->n - 1;
		if (phi->n == 1)
			second = t->n - 1;
		else
			second = phi->n - 2;
	}
	else
	{
		first = phi->n - 2;
		second = phi->n - 1;
	}
	if (fos == 'f')
		return (first);
	else if (fos == 's')
		return (second);
	else
		return (-1);
}
