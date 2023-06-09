/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:32:33 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/06/05 12:41:05 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

static int	handle_one(t_table *t)
{
	do_task((t_act){1, PICK, PI}, t);
	usleep(t->t_die);
	do_task((t_act){1, DEAD, DY}, t);
	return (exit_(0, t));
}

static void	*cycle(void *arg)
{
	unsigned long	time;
	t_philos		*p;
	int				cont;

	p = (t_philos *)arg;
	cont = 1;
	while (cont)
	{
		if (eat(p->t, p))
			return (NULL);
		sleep_(p);
		time = gtime();
		if (do_task((t_act){p->n, THINK, TH}, p->t))
			return (NULL);
		pthread_mutex_lock(&p->t->qmut);
		p->t->eaten++;
		if (p->t->n_eat != -1 && (long)(p->t->eaten > p->t->n * p->t->n_eat))
			cont = 0;
		pthread_mutex_unlock(&p->t->qmut);
		usleep((p->t->n * 250) - (gtime() - time) / 1000);
	}
	return (NULL);
}

static int	philo(t_table *t)
{
	pthread_t		reaper;
	int				i;

	t->s_time = gtime();
	if (t->n == 1)
		return (handle_one(t));
	t->philos = init_philo(t);
	if (!t->philos)
		return (exit_(2, t));
	pthread_create(&reaper, NULL, &death, t);
	i = -1;
	while (++i < t->n)
	{
		if (pthread_create(&t->philos[i].philo, NULL, &cycle,
				&t->philos[i]))
			return (exit_(4, t));
	}
	i = -1;
	if (pthread_join(reaper, NULL))
		return (exit_(5, t));
	while (++i < t->n)
		if (pthread_join(t->philos[i].philo, NULL))
			return (exit_(5, t));
	return (exit_(0, t));
}

int	main(int ac, char **av)
{
	t_table			*table;
	pthread_mutex_t	qmut;
	int				i;

	if (args_parser(ac, av))
		return (exit_(1, NULL));
	if (pthread_mutex_init(&qmut, NULL))
		return (exit_(3, NULL));
	table = malloc(sizeof(t_table));
	if (!table)
		return (exit_(2, NULL));
	*table = (t_table){stoi(av[1]), -1, 0, stoi(av[2]) * 1000, stoi(av[3])
		* 1000, stoi(av[4]) * 1000, 0, qmut, NULL,
		malloc(sizeof(pthread_mutex_t) * stoi(av[1])),
		malloc(sizeof(pthread_mutex_t) * stoi(av[1]))};
	if (!table->forks || ! table->reapers)
		return (exit_(2, table));
	if (ac == 6)
		table->n_eat = stoi(av[5]);
	i = -1;
	while (++i < table->n)
		if (pthread_mutex_init(&table->forks[i], NULL)
			|| pthread_mutex_init(&table->reapers[i], NULL))
			return (exit_(3, table));
	return (philo(table));
}
