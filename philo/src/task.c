/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 20:34:29 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/31 19:47:59 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

void	*death(void *arg)
{
	t_table	*t;
	int		cont;
	int		i;

	t = (t_table *)arg;
	usleep(t->t_die);
	cont = 1;
	while (cont)
	{
		i = -1;
		while (++i < t->n)
		{
			cont = t->philos[i].n_eat;
			if (gtime() - *(t->philos[i].last_eat) >= t->t_die)
			{
				do_task((t_act){t->philos[i].n, DEAD, DY, (gtime()
						- t->philos[i].t->s_time)}, t);
				return (NULL);
			}
		}
		usleep(5000);
	}
	return (NULL);
}

int	eat(t_table *t, t_philos *phi)
{
	pthread_mutex_t	*lock1;
	pthread_mutex_t	*lock2;

	lock1 = &t->forks[i_lock(t, phi, 'f')];
	lock2 = &t->forks[i_lock(t, phi, 's')];
	pthread_mutex_lock(lock1);
	if (do_task((t_act){phi->n, PICK, PI, (gtime() - t->s_time)}, t))
	{
		pthread_mutex_unlock(lock1);
		return (1);
	}
	pthread_mutex_lock(lock2);
	*(phi->last_eat) = gtime();
	do_task((t_act){phi->n, PICK, PI, (*(phi->last_eat) - t->s_time)}, t);
	do_task((t_act){phi->n, EAT, EA, *(phi->last_eat) - t->s_time}, t);
	usleep(t->t_eat);
	pthread_mutex_unlock(lock1);
	pthread_mutex_unlock(lock2);
	return (0);
}

int	do_task(t_act action, t_table *table)
{
	static int	rip = 0;

	if (rip)
		return (1);
	if (action.action == DEAD)
	{
		action.str = DY;
		rip = 1;
	}
	pthread_mutex_lock(&table->qmut);
	printf("%lu\t%d\t%s\n", action.time / 1000, action.philo, action.str);
	pthread_mutex_unlock(&table->qmut);
	return (0);
}
