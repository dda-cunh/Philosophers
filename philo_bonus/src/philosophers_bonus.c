/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:32:33 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/29 13:49:23 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers_bonus.h"

static int	*cycle(t_philos *p)
{
	unsigned long	interval;

	if (p->t->n_eat == 0)
		p->n_eat = 1;
	if (p->t->n == 1)
	{
		do_task((t_act){p->n, PICK, PI, (gtime() - p->t->s_time)}, p->t, p);
		usleep(p->t->t_die);
		do_task((t_act){p->n, DEAD, DY, (gtime() - p->t->s_time)}, p->t, p);
		return (0);
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
	return (0);
}

static int	philo(t_table *table)
{
	int	i;
	int	pid;

	table->s_time = gtime();
	table->philos = init_philo(table);
	if (!table->philos)
		return (exit_(2, table));
	i = -1;
	while (++i < table->n)
	{
		pid = fork();
		if (pid == -1)
			return (exit_(4, table));
		else if (pid == 0)
		{
			table->philos[i].pid = pid;
			cycle(&table->philos[i]);
			exit(0);
		}
	}
	waitpid(-1, NULL, 0);
	return (exit_(0, table));
}

int	main(int ac, char **av)
{
	t_table	table;
	sem_t	*print;
	sem_t	*forks;

	if (args_parser(ac, av))
		return (exit_(1, NULL));
	print = sem_open("/print", O_CREAT, 0777, 1);
	forks = sem_open("/forks", O_CREAT, 0777, stoi(av[1]));
	if (print == SEM_FAILED || forks == SEM_FAILED)
		return (exit_(3, NULL));
	if (ac == 5)
		table = (t_table){stoi(av[1]), 0, stoi(av[2]) * 1000, stoi(av[3])
			* 1000, stoi(av[4]) * 1000, 0, print, forks, NULL};
	else
		table = (t_table){stoi(av[1]), stoi(av[5]), stoi(av[2]) * 1000,
			stoi(av[3]) * 1000, stoi(av[4]) * 1000, 0, print, forks, NULL};
	return (philo(&table));
}
