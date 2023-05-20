/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 20:34:29 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/20 19:02:00 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

void	queue_philo(t_philos **head, t_philos *philo)
{
	t_philos	*current;

	if (head && philo)
	{
		if (*head)
		{
			current = *head;
			while (current->next)
				current = current->next;
			current->next = philo;
		}
		else
			*head = philo;
	}
	else
		if (philo)
			free(philo);
}

void	do_task(t_action action, t_table *table)
{
	pthread_mutex_lock(&table->qmut);
	if (action.action == THINK)
		printf("%lu %d is thinking\n", action.time, action.philo);
	else if (action.action == EAT)
		printf("%lu %d is eating\n", action.time, action.philo);
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
