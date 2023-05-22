/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:32:33 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/22 21:31:58 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"
#include <pthread.h>

void	*eat(void *arg);

void	*think(void *arg)
{
	t_table	*t;

	t = (t_table *)arg;
	do_task((t_action){t->philos->n, THINK, get_time_ms() - t->start_time}, t);
	usleep(t->t_eat);
	eat(t);
	return (NULL);
}

void	*sleep_(void *arg)
{
	t_table	*t;

	t = (t_table *)arg;
	do_task((t_action){t->philos->n, SLEEP, get_time_ms() - t->start_time}, t);
	usleep(t->t_sleep);
	think(t);
	return (NULL);
}

void	*eat(void *arg)
{
	t_table	*t;

	t = (t_table *)arg;
	if (t->philos->n == 1)
	{
		pthread_mutex_lock(&t->forks[t->n - 1]);
		pthread_mutex_lock(&t->forks[t->philos->n - 1]);
		do_task((t_action){t->philos->n, EAT, get_time_ms() - t->start_time},
			t);
		usleep(t->t_eat);
		pthread_mutex_unlock(&t->forks[t->n - 1]);
		pthread_mutex_unlock(&t->forks[t->philos->n - 1]);
	}
	else
	{
		pthread_mutex_lock(&t->forks[t->philos->n - 2]);
		pthread_mutex_lock(&t->forks[t->philos->n - 1]);
		do_task((t_action){t->philos->n, EAT, get_time_ms() - t->start_time},
			t);
		usleep(t->t_eat);
		pthread_mutex_unlock(&t->forks[t->philos->n - 2]);
		pthread_mutex_unlock(&t->forks[t->philos->n - 1]);
	}
	sleep_(t);
	return (NULL);
}

int	philo(t_table *table)
{
	int			i;
	t_philos	*philos;
	t_philos	*start;

	i = 0;
	philos = new_philo(1);
	start = philos;
	while (++i <= table->n)
	{
		table->philos = philos;
		if (i % 2)
		{
			if (pthread_create(&philos->philo, NULL, &eat, table))
				return (exit_(4, table));
		}
		else
			if (pthread_create(&philos->philo, NULL, &think, table))
				return (exit_(4, table));
		philos->next = new_philo(i + 1);
		philos = philos->next;
	}
	i = -1;
	table->philos = start;
	while (++i < table->n)
	{
		pthread_join(start->philo, NULL);
		start = start->next;
	}
	return (exit_(0, table));
}

int	main(int ac, char **av)
{
	int				i;
	t_table			table;
	pthread_mutex_t	qmut;

	if (args_parser(ac, av))
		return (exit_(1, NULL));
	if (pthread_mutex_init(&qmut, NULL))
		return (exit_(3, NULL));
	if (ac == 5)
		table = (t_table){stoi(av[1]), stoi(av[2]), stoi(av[3]), stoi(av[4]),
			0, get_time_ms(), qmut, NULL,
			malloc(sizeof(pthread_mutex_t) * stoi(av[1]))};
	else
		table = (t_table){stoi(av[1]), stoi(av[2]), stoi(av[3]), stoi(av[4]),
			0, get_time_ms(), qmut, NULL,
			malloc(sizeof(pthread_mutex_t) * stoi(av[1]))};
	if (!table.forks)
		return (exit_(2, &table));
	i = -1;
	while (++i < table.n)
		if (pthread_mutex_init(&table.forks[i], NULL))
			return (exit_(3, &table));
	return (philo(&table));
}
