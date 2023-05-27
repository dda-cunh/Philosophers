/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:32:33 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/27 14:41:51 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

static void	*cycle(void *arg)
{
	t_philos		*p;
	unsigned long	interval;

	p = (t_philos *)arg;
	if (p->t->n_eat == 0)
		p->n_eat = 1;
	if (p->t->n == 1)
	{
		do_task((t_act){p->n, PICK, PI, (gtime() - p->t->s_time)}, p->t, p);
		usleep(p->t->t_die);
		do_task((t_act){p->n, DEAD, DY, (gtime() - p->t->s_time)}, p->t, p);
		return (NULL);
	}
	while (p->n_eat)
	{
		if (eat(p->t, p))
			break ;
		if (sleep_(p->t, p))
			break ;
		interval = gtime();
		if (do_task((t_act){p->n, THINK, TH, interval - p->t->s_time}, p->t, p))
			break ;
		usleep((p->t->n * 250) - (gtime() - interval) / 1000);
	}
	return (NULL);
}

static int	philo(t_table *table)
{
	int			i;

	table->s_time = gtime();
	table->philos = init_philo(table);
	if (!table->philos)
		return (exit_(2, table));
	i = -1;
	while (++i < table->n)
	{
		if (pthread_create(&table->philos[i].philo, NULL, &cycle,
				&table->philos[i]))
			return (exit_(4, table));
	}
	i = -1;
	while (++i < table->n)
		if (pthread_join(table->philos[i].philo, NULL))
			return (exit_(5, table));
	return (exit_(0, table));
}

int	main(int ac, char **av)
{
	t_table			table;
	pthread_mutex_t	qmut;
	int				i;

	if (args_parser(ac, av))
		return (exit_(1, NULL));
	if (pthread_mutex_init(&qmut, NULL))
		return (exit_(3, NULL));
	if (ac == 5)
		table = (t_table){stoi(av[1]), 0, stoi(av[2]) * 1000, stoi(av[3])
			* 1000, stoi(av[4]) * 1000, 0, qmut, NULL,
			malloc(sizeof(pthread_mutex_t) * stoi(av[1]))};
	else
		table = (t_table){stoi(av[1]), stoi(av[5]), stoi(av[2]) * 1000, stoi(av[3]) * 1000,
			stoi(av[4]) * 1000, 0, qmut, NULL,
			malloc(sizeof(pthread_mutex_t) * stoi(av[1]))};
	if (!table.forks)
		return (exit_(2, &table));
	i = -1;
	while (++i < table.n)
		if (pthread_mutex_init(&table.forks[i], NULL))
			return (exit_(3, &table));
	return (philo(&table));
}
