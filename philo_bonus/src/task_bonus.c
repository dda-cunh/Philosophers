/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 20:34:29 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/06/01 15:17:20 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers_bonus.h"

void	*death(void *arg)
{
	t_philos	*p;
	int			cont;

	p = (t_philos *)arg;
	usleep(p->t->t_die);
	cont = 1;
	while (cont)
	{
		cont = p->n_eat;
		if (gtime() - p->last_eat >= p->t->t_die)
		{
			do_task((t_act){p->n, DEAD, DY, (gtime() - p->t->s_time)}, p->t);
			exit_(0, p->t);
			return (NULL);
		}
		usleep(5000);
	}
	return (NULL);
}

int	sleep_(t_table *t, t_philos *phi)
{
	if (do_task((t_act){phi->n, SLEEP, SL, gtime() - t->s_time}, t))
		return (1);
	usleep(t->t_sleep);
	return (0);
}

int	eat(t_table *t, t_philos *phi)
{
	sem_wait(t->forks);
	do_task((t_act){phi->n, PICK, PI, (gtime() - t->s_time)}, t);
	sem_wait(t->forks);
	do_task((t_act){phi->n, PICK, PI, (gtime() - t->s_time)}, t);
	phi->last_eat = gtime();
	do_task((t_act){phi->n, EAT, EA, (gtime() - t->s_time)}, t);
	usleep(t->t_eat);
	sem_post(t->forks);
	sem_post(t->forks);
	return (0);
}

int	do_task(t_act action, t_table *table)
{
	static int	rip = 0;

	if (rip)
		return (1);
	sem_wait(table->print);
	if (action.action == DEAD)
	{
		rip = 1;
		action.str = DY;
	}
	printf("%lu\t%d\t%s\n", action.time / 1000, action.philo, action.str);
	sem_post(table->print);
	return (0);
}
