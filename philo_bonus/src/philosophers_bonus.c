/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:32:33 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/06/02 15:51:59 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers_bonus.h"

static int	*cycle(t_philos *p)
{
	unsigned long	interval;
	pthread_t		reaper;

	if (p->t->n_eat == 0)
		p->n_eat = -1;
	pthread_create(&reaper, NULL, &death, p);
	pthread_detach(reaper);
	if (p->t->n == 1)
	{
		do_task((t_act){p->n, PICK, PI, (gtime() - p->t->s_time)}, p->t);
		usleep(p->t->t_die);
		return (0);
	}
	while (p->n_eat)
	{
		if (eat(p->t, p))
			break ;
		sleep_(p->t, p);
		interval = gtime();
		if (do_task((t_act){p->n, THINK, TH, interval - p->t->s_time}, p->t))
			break ;
		usleep((p->t->n * 250) - (gtime() - interval) / 1000);
	}
	return (0);
}

static int	philo(t_table *table)
{
	t_philos	philo;
	int			i;

	table->s_time = gtime();
	i = -1;
	while (++i < table->n)
	{
		philo = (t_philos){i + 1, table->n_eat, table->s_time, fork(), table};
		if (philo.pid == -1)
			return (exit_(4, table));
		else if (philo.pid == 0)
		{
			cycle(&philo);
			exit(0);
		}
	}
	waitpid(-1, NULL, 0);
	if (table->n_eat)
		usleep(table->t_eat);
	return (exit_(0, table));
}

int	main(int ac, char **av)
{
	t_table	table;
	sem_t	*print;
	sem_t	*forks;
	sem_t	*reapers;

	if (args_parser(ac, av))
		return (exit_(1, NULL));
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/reapers");
	print = sem_open("/print", O_CREAT, 0777, 1);
	forks = sem_open("/forks", O_CREAT, 0777, stoi(av[1]));
	reapers = sem_open("/reapers", O_CREAT, 0777, 1);
	if (print == SEM_FAILED || forks == SEM_FAILED)
		return (exit_(3, NULL));
	table = (t_table){stoi(av[1]), 0, 0, stoi(av[2]) * 1000, stoi(av[3])
		* 1000, stoi(av[4]) * 1000, 0, print, forks, reapers};
	if (ac == 6)
		table.n_eat = stoi(av[5]);
	return (philo(&table));
}
