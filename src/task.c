/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 20:34:29 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/24 19:09:12 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

int	do_task(t_act action, t_table *table, t_philos *philo)
{
	pthread_mutex_lock(&table->qmut);
	action.time /= 1000;
	if (table->rip == 0)
	{
		if (gtime() - philo->last_eat >= table->t_die)
		{
			table->rip = 1;
			action.action = DEAD;
			action.str = DYING;
		}
		printf("[%lu]\t%d %s\n", action.time, action.philo, action.str);
		if (action.action == EAT)
			philo->last_eat = gtime();
	}
	pthread_mutex_unlock(&table->qmut);
	return (table->rip);
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
