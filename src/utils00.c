/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils00.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 15:09:37 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/25 14:15:04 by dda-cunh         ###   ########.fr       */
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
	int			i;
	t_philos	*old;

	if (table)
	{
		if (table->forks)
		{
			i = -1;
			while (++i < table->n)
				pthread_mutex_destroy(&(table->forks[i]));
			free(table->forks);
		}
		if (table->philos)
		{
			while (table->philos)
			{
				old = table->philos;
				table->philos = table->philos->next;
				free(old);
			}
		}
	}
}

int	exit_(int status, t_table *table)
{
	clean(table);
	if (status == 1)
		printf("Bad args\n");
	else if (status == 2)
		printf("Error on Malloc\n");
	else if (status == 3)
		printf("Error on mutex creation\n");
	else if (status == 4)
		printf("Error on pthread creation\n");
	return (status);
}

t_philos	*new_philo(int n)
{
	t_philos	*philo;

	philo = malloc(sizeof(t_philos));
	if (!philo)
		return (NULL);
	philo->n = n;
	philo->next = NULL;
	return (philo);
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
