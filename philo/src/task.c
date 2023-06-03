/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 20:34:29 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/06/03 15:17:08 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

static int	stopeat(t_table *t)
{
	pthread_mutex_lock(&t->qmut);
	if (t->eaten > t->n * t->n_eat)
	{
		pthread_mutex_unlock(&t->qmut);
		return (1);
	}
	pthread_mutex_unlock(&t->qmut);
	return (0);
}

void	*death(void *arg)
{
	t_table	*t;
	int		i;

	t = (t_table *)arg;
	usleep(t->t_die);
	while (1)
	{
		i = -1;
		while (++i < t->n)
		{
			if (stopeat(t))
				return (NULL);
			pthread_mutex_lock(&t->reapers[i]);
			if (gtime() - *(t->philos[i].last_eat) >= t->t_die)
			{
				do_task((t_act){t->philos[i].n, DEAD, DY, (gtime()
						- t->philos[i].t->s_time)}, t);
				pthread_mutex_unlock(&t->reapers[i]);
				return (NULL);
			}
			pthread_mutex_unlock(&t->reapers[i]);
		}
	}
	return (NULL);
}

int	sleep_(t_philos *phi)
{
	unsigned long	time;

	time = gtime();
	if (do_task((t_act){phi->n, SLEEP, SL, time - phi->t->s_time}, phi->t))
		return (1);
	usleep(phi->t->t_sleep);
	return (0);
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
	pthread_mutex_lock(&phi->t->reapers[phi->n - 1]);
	*(phi->last_eat) = gtime();
	pthread_mutex_unlock(&phi->t->reapers[phi->n - 1]);
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

	pthread_mutex_lock(&table->qmut);
	if (rip)
	{
		pthread_mutex_unlock(&table->qmut);
		return (1);
	}
	if (action.action == DEAD)
	{
		action.str = DY;
		rip = 1;
	}
	printf("%lu\t%d\t%s\n", action.time / 1000, action.philo, action.str);
	pthread_mutex_unlock(&table->qmut);
	return (0);
}
