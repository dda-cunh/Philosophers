/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 20:34:29 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/27 13:29:52 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

int	sleep_(t_table *t, t_philos *phi)
{
	if (do_task((t_act){phi->n, SLEEP, SL, gtime() - t->s_time}, t, phi))
		return (1);
	usleep(t->t_sleep);
	return (0);
}

int	eat(t_table *t, t_philos *phi)
{
	pthread_mutex_t	*lock1;
	pthread_mutex_t	*lock2;

	lock1 = &t->forks[i_lock(t, phi, 'f')];
	lock2 = &t->forks[i_lock(t, phi, 's')];
	pthread_mutex_lock(lock1);
	do_task((t_act){phi->n, PICK, PI, (gtime() - t->s_time)}, t, phi);
	pthread_mutex_lock(lock2);
	do_task((t_act){phi->n, PICK, PI, (gtime() - t->s_time)}, t, phi);
	do_task((t_act){phi->n, EAT, EA, (gtime() - t->s_time)}, t, phi);
	usleep(t->t_eat);
	pthread_mutex_unlock(lock1);
	pthread_mutex_unlock(lock2);
	return (0);
}

int	do_task(t_act action, t_table *table, t_philos *philo)
{
	static int	rip = 0;

	pthread_mutex_lock(&table->qmut);
	if (rip == 0)
	{
		if ((gtime() - philo->last_eat) >= table->t_die)
		{
			rip = 1;
			action.action = DEAD;
			action.str = DY;
		}
		printf("%lu\t%d\t%s\n", action.time / 1000, action.philo, action.str);
		if (action.action == EAT)
		{
			if (table->n_eat)
				philo->n_eat--;
			philo->last_eat = gtime();
		}
	}
	pthread_mutex_unlock(&table->qmut);
	return (rip);
}
