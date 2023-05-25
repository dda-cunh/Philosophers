/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:32:33 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/25 20:26:48 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

static void	*cycle(void *arg)
{
	t_philos		*p;

	p = (t_philos *)arg;
	p->n_eat = p->t->n_eat;
	if (p->n_eat == 0)
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
		if (do_task((t_act){p->n, THINK, TH, gtime() - p->t->s_time}, p->t, p))
			break ;
	}
	return (NULL);
}

static int	philo(t_table *table)
{
	t_philos	*philos;
	int			i;

	table->philos_start = new_philo(1);
	philos = table->philos_start;
	i = 0;
	table->s_time = gtime();
	while (++i <= table->n)
	{
		philos->last_eat = table->s_time;
		philos->t = table;
		if (pthread_create(&philos->philo, NULL, &cycle, philos))
			return (exit_(4, table));
		philos->next = new_philo(i + 1);
		philos = philos->next;
	}
	i = -1;
	philos = table->philos_start;
	while (++i < table->n)
	{
		pthread_join(philos->philo, NULL);
		philos = philos->next;
	}
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
		table = (t_table){stoi(av[1]), stoi(av[2]) * 1000, stoi(av[3])
			* 1000, stoi(av[4]) * 1000, 0, 0, qmut, NULL,
			malloc(sizeof(pthread_mutex_t) * stoi(av[1]))};
	else
		table = (t_table){stoi(av[1]), stoi(av[2]) * 1000, stoi(av[3])
			* 1000, stoi(av[4]) * 1000, stoi(av[5]), 0, qmut,
			NULL, malloc(sizeof(pthread_mutex_t) * stoi(av[1]))};
	if (!table.forks)
		return (exit_(2, &table));
	i = -1;
	while (++i < table.n)
		if (pthread_mutex_init(&table.forks[i], NULL))
			return (exit_(3, &table));
	return (philo(&table));
}
