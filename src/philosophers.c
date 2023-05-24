/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:32:33 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/24 19:01:56 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

void	*eat(void *arg)
{
	t_philos		*phi;
	t_table			*t;
	pthread_mutex_t	*lock1;
	pthread_mutex_t	*lock2;

	t = (t_table *)arg;
	phi = t->philos;
	phi->last_eat = t->s_time;
	if (t->n == 1)
	{
		do_task((t_act){phi->n, PICK, PICKING, (gtime() - t->s_time)}, t, phi);
		usleep(t->t_die);
		do_task((t_act){phi->n, DEAD, DYING, (gtime() - t->s_time)}, t, phi);
		return (NULL);
	}
	if (t->n % 2)
	{
		lock1 = &t->forks[phi->n - 1];
		if (phi->n == 1)
			lock2 = &t->forks[t->n - 1];
		else
			lock2 = &t->forks[phi->n - 2];
	}
	else
	{
		lock1 = &t->forks[phi->n - 2];
		lock2 = &t->forks[phi->n - 1];
	}
	while (t->rip == 0)
	{
		pthread_mutex_lock(lock1);
		do_task((t_act){phi->n, PICK, PICKING, (gtime() - t->s_time)}, t, phi);
		pthread_mutex_lock(lock2);
		do_task((t_act){phi->n, PICK, PICKING, (gtime() - t->s_time)}, t, phi);
		do_task((t_act){phi->n, EAT, EATING, (gtime() - t->s_time)}, t, phi);
		usleep(t->t_eat);
		pthread_mutex_unlock(lock1);
		pthread_mutex_unlock(lock2);
		do_task((t_act){phi->n, SLEEP, SLEEPING, gtime() - t->s_time}, t, phi);
		usleep(t->t_sleep);
		do_task((t_act){phi->n, THINK, THINKING, gtime() - t->s_time}, t, phi);
	}
	return (NULL);
}

int	philo(t_table *table)
{
	t_philos	*philos;
	int			i;

	i = 0;
	philos = new_philo(1);
	table->philos_start = philos;
	while (++i <= table->n)
	{
		table->philos = philos;
		if (pthread_create(&philos->philo, NULL, &eat, table))
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
