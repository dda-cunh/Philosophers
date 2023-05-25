/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:32:33 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/25 14:31:02 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

void	*cycle(void *arg)
{
	t_philos		*phi;
	t_table			*t;

	t = (t_table *)arg;
	phi = t->philos;
	phi->n_eat = t->n_eat;
	if (phi->n_eat == 0)
		phi->n_eat = 1;
	if (t->n == 1)
	{
		do_task((t_act){phi->n, PICK, PI, (gtime() - t->s_time)}, t, phi);
		usleep(t->t_die);
		do_task((t_act){phi->n, DEAD, DY, (gtime() - t->s_time)}, t, phi);
		return (NULL);
	}
	while (t->rip == 0 && phi->n_eat)
	{
		if (eat(t, phi))
			break ;
		if (sleep_(t, phi))
			break ;
		if (do_task((t_act){phi->n, THINK, TH, gtime() - t->s_time}, t, phi))
			break ;
	}
	return (NULL);
}

int	philo(t_table *table)
{
	t_philos	*philos;
	int			i;

	table->philos_start = new_philo(1);
	philos = table->philos_start;
	i = 0;
	while (++i <= table->n)
	{
		table->philos = philos;
		philos->last_eat = table->s_time;
		if (pthread_create(&philos->philo, NULL, &cycle, table))
			return (exit_(4, table));
		usleep(1);
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
	table->philos = table->philos_start;
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
			* 1000, stoi(av[4]) * 1000, 0, gtime(), qmut, NULL, NULL,
			malloc(sizeof(pthread_mutex_t) * stoi(av[1]))};
	else
		table = (t_table){stoi(av[1]), 0, stoi(av[2]) * 1000, stoi(av[3])
			* 1000, stoi(av[4]) * 1000, stoi(av[5]), gtime(), qmut, NULL,
			NULL, malloc(sizeof(pthread_mutex_t) * stoi(av[1]))};
	if (!table.forks)
		return (exit_(2, &table));
	i = -1;
	while (++i < table.n)
		if (pthread_mutex_init(&table.forks[i], NULL))
			return (exit_(3, &table));
	return (philo(&table));
}
