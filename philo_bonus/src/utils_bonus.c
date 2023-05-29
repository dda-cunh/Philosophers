/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 15:09:37 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/29 13:50:01 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers_bonus.h"
#include <signal.h>

unsigned long	gtime(void)
{
	unsigned long	ms;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	ms = (unsigned long)(tv.tv_sec)*1000
		+ (unsigned long)(tv.tv_usec) / 1000;
	return (ms * 1000);
}

static void	clean(t_table *table)
{
	if (table)
	{
		sem_unlink("/forks");
		sem_unlink("/print");
		if (table->philos)
			free(table->philos);
	}
}

int	exit_(int status, t_table *table)
{
	clean(table);
	if (!status)
		kill(0, SIGKILL);
	if (status == 1)
		printf("Bad args\n");
	else if (status == 2)
		printf("Error on Malloc\n");
	else if (status == 3)
		printf("Error on sem creation\n");
	else if (status == 4)
		printf("Error on philo creation\n");
	// else if (status == 5)
	// 	printf("Error on pthread join\n");
	return (status);
}

t_philos	*init_philo(t_table *table)
{
	int			i;
	t_philos	*phi;

	phi = malloc(sizeof(t_philos) * table->n);
	if (!phi)
		return (NULL);
	i = -1;
	while (++i < table->n)
		phi[i] = (t_philos){i + 1, table->n_eat, table->s_time, 0, table};
	return (phi);
}
