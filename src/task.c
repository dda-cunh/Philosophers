/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 20:34:29 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/23 20:22:04 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

int	do_task(t_act action, t_table *table, t_philos *philo)
{
	pthread_mutex_lock(&table->qmut);
	action.time /= 1000;
	if (get_time_ms() - philo->last_eat >= table->t_die)
	{
		table->rip = 1;
		action.action = DEAD;
	}
	if (action.action == THINK)
		printf("%lu %d is thinking\n", action.time, action.philo);
	else if (action.action == EAT)
	{
		printf("%lu %d is eating\n", action.time, action.philo);
		philo->last_eat = get_time_ms();
	}
	else if (action.action == SLEEP)
		printf("%lu %d is sleeping\n", action.time, action.philo);
	else if (action.action == PICK)
		printf("%lu %d has taken a fork\n", action.time,
			action.philo);
	else if (action.action == DROP)
		printf("%lu %d has dropped a fork\n", action.time, action.philo);
	else if (action.action == DEAD)
		printf("%lu %d died\n", action.time, action.philo);
	pthread_mutex_unlock(&table->qmut);
	return (action.action == DEAD);
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
