/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils00.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 15:09:37 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/19 16:10:55 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"
#include <stdio.h>

static void	clean(t_table *table)
{
	if (table)
	{
		if (table->forks)
			free(table->forks);
		if (table->philos)
			free(table->philos);
	}
	return ;
}

int	exit_(int status, t_table *table)
{
	clean(table);
	if (status == 1)
		printf("Bad args\n");
	return (status);
}
