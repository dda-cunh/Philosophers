/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:32:33 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/19 21:13:05 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

int	philo(t_table *table)
{
	return (exit_(0, table));
}

int	main(int ac, char **av)
{
	t_table	table;
	int		i;

	if (args_parser(ac, av))
		return (exit_(1, NULL));
	if (ac == 5)
		table = (t_table){stoi(av[1]), stoi(av[2]), stoi(av[3]), stoi(av[4]),
			0, 0, get_time_ms(), NULL, malloc(sizeof(pthread_t) * stoi(av[1])),
			malloc(sizeof(pthread_mutex_t) * stoi(av[1]))};
	else
		table = (t_table){stoi(av[1]), stoi(av[2]), stoi(av[3]), stoi(av[4]),
			stoi(av[5]), 0, get_time_ms(), NULL, malloc(sizeof(pthread_t)
				* stoi(av[1])), malloc(sizeof(pthread_mutex_t) * stoi(av[1]))};
	if (!table.philos || !table.forks)
		return (exit_(2, &table));
	i = -1;
	while (++i < table.n)
		if (pthread_mutex_init(&table.forks[i], NULL))
			return (exit_(3, &table));
	return (philo(&table));
}
