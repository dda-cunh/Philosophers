/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 20:34:29 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/20 00:19:12 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

void	queue_task(t_taskQueue **head, t_taskQueue	*task)
{
	t_taskQueue	*current;

	if (head && task)
	{
		if (*head)
		{
			current = *head;
			while (current->next)
				current = current->next;
			current->next = task;
		}
		else
			*head = task;
	}
	else
		if (task)
			free(task);
}

void	unqueue_task(t_taskQueue **t)
{
	t_taskQueue	*head;

	head = (*t)->next;
	free(*t);
	*t = head;
}

void	do_task(t_taskQueue **t)
{
	if ((*t)->action.action == THINK)
		printf("%lu %d is thinking\n", (*t)->action.time, (*t)->action.philo);
	else if ((*t)->action.action == EAT)
		printf("%lu %d is eating\n", (*t)->action.time, (*t)->action.philo);
	else if ((*t)->action.action == SLEEP)
		printf("%lu %d is sleeping\n", (*t)->action.time, (*t)->action.philo);
	else if ((*t)->action.action == PICK)
		printf("%lu %d has taken a fork\n", (*t)->action.time,
			(*t)->action.philo);
	else if ((*t)->action.action == DROP)
		printf("%lu %d has dropped a fork\n", (*t)->action.time,
			(*t)->action.philo);
	unqueue_task(t);
}

t_taskQueue	*new_task(t_action action)
{
	t_taskQueue	*task;

	task = malloc(sizeof(t_taskQueue));
	if (!task)
		return (NULL);
	task->action = action;
	task->next = NULL;
	return (task);
}
