/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:32:33 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/06/02 16:01:04 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

static void	*cycle(void *arg)
{
	t_philos		*p;
	unsigned long	time;

	p = (t_philos *)arg;
	if (p->t->n == 1)
	{
		do_task((t_act){1, PICK, PI, gtime() - p->t->s_time}, p->t);
		usleep(p->t->t_die);
		return (NULL);
	}
	while (p->n_eat)
	{
		if (eat(p->t, p))
			return (NULL);
		time = gtime();
		if (do_task((t_act){p->n, SLEEP, SL, time - p->t->s_time}, p->t))
			return (NULL);
		usleep(p->t->t_sleep - (gtime() - time));
		time = gtime();
		if (do_task((t_act){p->n, THINK, TH, time - p->t->s_time}, p->t))
			return (NULL);
		usleep((p->t->n * 250) - (gtime() - time) / 1000);
	}
	return (NULL);
}

static int	philo(t_table *t)
{
	pthread_t		reaper;
	int				i;

	t->philos = init_philo(t);
	if (!t->philos)
		return (exit_(2, t));
	t->s_time = gtime();
	pthread_create(&reaper, NULL, &death, t);
	i = -1;
	while (++i < t->n)
	{
		t->philos[i].n_eat = t->n_eat;
		if (t->n_eat == 0)
			t->philos[i].n_eat = -1;
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
	*table = (t_table){stoi(av[1]), 0, stoi(av[2]) * 1000, stoi(av[3]) * 1000,
		stoi(av[4]) * 1000, 0, qmut, NULL, malloc(sizeof(pthread_mutex_t)
			* stoi(av[1])), malloc(sizeof(pthread_mutex_t) * stoi(av[1]))};
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
