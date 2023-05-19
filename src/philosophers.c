/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 15:32:33 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/19 16:18:04 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

int	main(int ac, char **av)
{
	t_table	table;

	if (args_parser(ac, av))
		return (exit_(1, NULL));
	table.n = stoi(av[1]);
	table.philos = malloc(sizeof(pthread_t) * table.n);
	table.forks = malloc(sizeof(pthread_mutex_t) * table.n);
	return (exit_(0, &table));
}
