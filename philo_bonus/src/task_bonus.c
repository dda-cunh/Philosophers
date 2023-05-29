/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 20:34:29 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/29 13:20:51 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers_bonus.h"

int	sleep_(t_table *t, t_philos *phi)
{
	if (do_task((t_act){phi->n, SLEEP, SL, gtime() - t->s_time}, t, phi))
		return (1);
	usleep(t->t_sleep);
	return (0);
}

int	eat(t_table *t, t_philos *phi)
{
	sem_wait(t->forks);
	do_task((t_act){phi->n, PICK, PI, (gtime() - t->s_time)}, t, phi);
	sem_wait(t->forks);
	do_task((t_act){phi->n, PICK, PI, (gtime() - t->s_time)}, t, phi);
	do_task((t_act){phi->n, EAT, EA, (gtime() - t->s_time)}, t, phi);
	usleep(t->t_eat);
	sem_post(t->forks);
	sem_post(t->forks);
	return (0);
}

int	do_task(t_act action, t_table *table, t_philos *philo)
{
	static int	rip = 0;

	sem_wait(table->print);
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
	sem_post(table->print);
	return (rip);
}
