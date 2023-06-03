/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 14:39:37 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/06/03 15:51:15 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

# define TH "is thinking"
# define EA "is eating"
# define SL "is sleeping"
# define PI "has taken a fork"
# define DY "died"

typedef struct s_table	t_table;

typedef enum e_state
{
	THINK = 0,
	EAT = 1,
	SLEEP = 2,
	PICK = 3,
	DEAD = 4
}	t_state;

typedef struct s_act
{
	int				philo;
	t_state			action;
	char			*str;
}	t_act;

typedef struct s_philos
{
	int				n;
	unsigned long	*last_eat;
	pthread_t		philo;
	t_table			*t;
}	t_philos;

typedef struct s_table
{
	int				n;
	int				n_eat;
	int				eaten;
	unsigned long	t_die;
	unsigned long	t_eat;
	unsigned long	t_sleep;
	unsigned long	s_time;
	pthread_mutex_t	qmut;
	t_philos		*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*reapers;
}	t_table;

/* ************************************************************************** */
/*                                                                            */
/*                          MAIN: philosophers.c                              */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                 parser.c                                   */
/* ************************************************************************** */
int				sncmp(const char *s1, const char *s2, unsigned int n);
int				args_parser(int ac, char **av);
int				stoi(const char *nptr);
int				sisint(const char *s);
int				sisnum(const char *s);

/* ************************************************************************** */
/*                                  utils.c                                   */
/* ************************************************************************** */
int				i_lock(t_table *t, t_philos *phi, char fos);
int				exit_(int status, t_table *table);
unsigned long	gtime(void);
t_philos		*init_philo(t_table *table);

/* ************************************************************************** */
/*                                 task.c                                     */
/* ************************************************************************** */
void			*death(void *arg);
int				do_task(t_act action, t_table *table);
int				sleep_(t_philos *phi);
int				eat(t_table *t, t_philos *phi);

#endif