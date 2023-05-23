/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:32:33 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/23 20:31:49 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

void	*eat(void *arg);

void	*eat(void *arg)
{
	pthread_mutex_t	*lock;
	t_philos		*phi;
	t_table			*tt;
	t_table			t;

	tt = (t_table *)arg;
	phi = tt->philos;
	t = (t_table){tt->n, tt->rip, tt->t_die, tt->t_eat, tt->t_sleep,
		tt->n_eat, tt->s_time, tt->qmut, tt->philos_start, phi, tt->forks};
	while (t.rip == 0)
	{
		if (phi->n == 1)
			lock = (pthread_mutex_t []){t.forks[t.n - 1],
				t.forks[0]};
		else
			lock = (pthread_mutex_t []){t.forks[phi->n - 2],
				t.forks[phi->n - 1]};
		pthread_mutex_lock(&lock[0]);
		do_task((t_act){phi->n, PICK, (get_time_ms() - t.s_time)}, &t, phi);
		pthread_mutex_lock(&lock[1]);
		do_task((t_act){phi->n, PICK, (get_time_ms() - t.s_time)}, &t, phi);
		do_task((t_act){phi->n, EAT, (get_time_ms() - t.s_time)}, &t, phi);
		usleep(t.t_eat);
		pthread_mutex_unlock(&lock[0]);
		do_task((t_act){phi->n, DROP, (get_time_ms() - t.s_time)}, &t, phi);
		pthread_mutex_unlock(&lock[1]);
		do_task((t_act){phi->n, DROP, (get_time_ms() - t.s_time)}, &t, phi);
		if (do_task((t_act){phi->n, SLEEP, get_time_ms() - t.s_time}, &t, phi))
			return (NULL);
		usleep(t.t_sleep);
		if (do_task((t_act){phi->n, THINK, get_time_ms() - t.s_time}, &t, phi))
			return (NULL);
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
		philos->last_eat = table->s_time;
		if (pthread_create(&philos->philo, NULL, &eat, table))
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
			* 1000, stoi(av[4]) * 1000,
			0, get_time_ms(), qmut, NULL, NULL,
			malloc(sizeof(pthread_mutex_t) * stoi(av[1]))};
	else
		table = (t_table){stoi(av[1]), 0, stoi(av[2]) * 1000, stoi(av[3])
			* 1000, stoi(av[4]) * 1000,
			stoi(av[5]) * 1000, get_time_ms(), qmut, NULL, NULL,
			malloc(sizeof(pthread_mutex_t) * stoi(av[1]))};
	if (!table.forks)
		return (exit_(2, &table));
	i = -1;
	while (++i < table.n)
		if (pthread_mutex_init(&table.forks[i], NULL))
			return (exit_(3, &table));
	return (philo(&table));
}
