/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils00.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 15:09:37 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/20 19:02:29 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

unsigned long	get_time_ms(void)
{
	unsigned long	ms;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	ms = (unsigned long)(tv.tv_sec)*1000
		+ (unsigned long)(tv.tv_usec) / 1000;
	return (ms);
}

static void	clean(t_table *table)
{
	int			i;
	t_philos	*old;

	if (table)
	{
		if (table->forks)
		{
			i = -1;
			while (++i < table->n)
				pthread_mutex_destroy(&(table->forks[i]));
			free(table->forks);
		}
		if (table->philos)
		{
			while (table->philos)
			{
				old = table->philos;
				table->philos = table->philos->next;
				free(old);
			}
		}
	}
}

int	exit_(int status, t_table *table)
{
	clean(table);
	if (status == 1)
		printf("Bad args\n");
	else if (status == 2)
		printf("Error on Malloc\n");
	else if (status == 3)
		printf("Error on mutex creation\n");
	else if (status == 4)
		printf("Error on pthread creation\n");
	return (status);
}
