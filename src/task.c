/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 20:34:29 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/19 21:10:57 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

void	do_task(t_taskQueue *task)
{
	if (task->action.action == THINK)
		printf("%lu %d is thinking\n", task->action.time, task->action.philo);
	else if (task->action.action == EAT)
		printf("%lu %d is eating\n", task->action.time, task->action.philo);
	else if (task->action.action == SLEEP)
		printf("%lu %d is sleeping\n", task->action.time, task->action.philo);
	else if (task->action.action == PICK)
		printf("%lu %d has taken a fork\n", task->action.time,
			task->action.philo);
	else if (task->action.action == DROP)
		printf("%lu %d has dropped a fork\n", task->action.time,
			task->action.philo);
}

void	queue_task(t_taskQueue **head, t_taskQueue	*task)
{
	if (head)
	{
		if (*head)
		{
			while ((*head)->next)
				(*head) = ((*head)->next);
			(*head)->next = task;
		}
		else
			if (task)
				free(task);
	}
	else
		if (task)
			free(task);
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
