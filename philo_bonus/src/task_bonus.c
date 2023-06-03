/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 20:34:29 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/06/03 15:53:24 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers_bonus.h"
#include <semaphore.h>

void	*death(void *arg)
{
	t_philos	*p;
	int			cont;

	p = (t_philos *)arg;
	usleep(p->t->t_die);
	cont = 1;
	while (cont)
	{
		sem_wait(p->t->reapers);
		cont = p->n_eat;
		if (gtime() - p->last_eat >= p->t->t_die)
		{
			do_task((t_act){p->n, DEAD, DY}, p->t);
			p->t->rip = 1;
			sem_post(p->t->reapers);
			exit_(0, p->t);
			return (NULL);
		}
		sem_post(p->t->reapers);
		usleep(5000);
	}
	return (NULL);
}

int	sleep_(t_table *t, t_philos *phi)
{
	if (do_task((t_act){phi->n, SLEEP, SL}, t))
		return (1);
	usleep(t->t_sleep);
	return (0);
}

int	eat(t_table *t, t_philos *phi)
{
	sem_wait(t->forks);
	do_task((t_act){phi->n, PICK, PI}, t);
	sem_wait(t->forks);
	do_task((t_act){phi->n, PICK, PI}, t);
	sem_wait(phi->t->reapers);
	phi->last_eat = gtime();
	phi->n_eat--;
	sem_post(phi->t->reapers);
	do_task((t_act){phi->n, EAT, EA}, t);
	usleep(t->t_eat);
	sem_post(t->forks);
	sem_post(t->forks);
	return (0);
}

int	do_task(t_act action, t_table *table)
{
	unsigned long	frame;
	static int		rip = 0;

	sem_wait(table->print);
	if (rip)
		return (1);
	frame = gtime() - table->s_time;
	if (action.action == DEAD)
	{
		rip = 1;
		action.str = DY;
	}
	if (table->rip == 0)
		printf("%lu\t%d\t%s\n", frame / 1000, action.philo, action.str);
	sem_post(table->print);
	return (0);
}
