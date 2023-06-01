/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-cunh <dda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 14:39:37 by dda-cunh          #+#    #+#             */
/*   Updated: 2023/06/01 14:43:57 by dda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <semaphore.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>

# define PI "has taken a fork"
# define SL "is sleeping"
# define TH "is thinking"
# define EA "is eating"
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
	unsigned long	time;
}	t_act;

typedef struct s_philos
{
	int				n;
	int				n_eat;
	unsigned long	last_eat;
	int				pid;
	t_table			*t;
}	t_philos;

typedef struct s_table
{
	int				n;
	int				n_eat;
	unsigned long	t_die;
	unsigned long	t_eat;
	unsigned long	t_sleep;
	unsigned long	s_time;
	sem_t			*print;
	sem_t			*forks;
}	t_table;

/* ************************************************************************** */
/*                                                                            */
/*                         MAIN: philosophers_bonus.c                         */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                              parser_bonus.c                                */
/* ************************************************************************** */
int				sncmp(const char *s1, const char *s2, unsigned int n);
int				args_parser(int ac, char **av);
int				stoi(const char *nptr);
int				sisint(const char *s);
int				sisnum(const char *s);

/* ************************************************************************** */
/*                                utils_bonus.c                               */
/* ************************************************************************** */
int				i_lock(t_table *t, t_philos *phi, char fos);
int				exit_(int status, t_table *table);
t_philos		*init_philo(t_table *table);
unsigned long	gtime(void);

/* ************************************************************************** */
/*                                 task_bonus.c                               */
/* ************************************************************************** */
void			*death(void *arg);
int				do_task(t_act action, t_table *table);
int				sleep_(t_table *t, t_philos *phi);
int				eat(t_table *t, t_philos *phi);

#endif