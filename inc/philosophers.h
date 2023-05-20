/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 14:39:37 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/05/20 18:51:17 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef enum e_state
{
	THINK = 0,
	EAT = 1,
	SLEEP = 2,
	PICK = 3,
	DROP = 4,
	DEAD = 5
}	t_state;

typedef struct s_action
{
	int				philo;
	t_state			action;
	unsigned long	time;
}	t_action;

typedef struct s_philos
{
	int				n;
	pthread_t		philo;
	struct s_philos	*next;
}	t_philos;

typedef struct s_table
{
	int				n;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				n_eat;
	unsigned long	start_time;
	pthread_mutex_t	qmut;
	t_philos		*philos;
	pthread_mutex_t	*forks;
}	t_table;

/* ************************************************************************** */
/*                                 parser.c                                   */
/* ************************************************************************** */
int				sncmp(const char *s1, const char *s2, unsigned int n);
int				args_parser(int ac, char **av);
int				stoi(const char *nptr);
int				sisint(const char *s);
int				sisnum(const char *s);

/* ************************************************************************** */
/*                                utils00.c                                   */
/* ************************************************************************** */
int				exit_(int status, t_table *table);
unsigned long	get_time_ms(void);

/* ************************************************************************** */
/*                                 task.c                                     */
/* ************************************************************************** */
void			queue_philo(t_philos **head, t_philos	*task);
void			do_task(t_action action, t_table *table);
void			clear_philos(t_philos **t);
void			*queue(void *args);
t_philos		*new_philo(int n);

#endif