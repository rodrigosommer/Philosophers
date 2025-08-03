/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:24:19 by rsommer           #+#    #+#             */
/*   Updated: 2025/05/17 17:24:20 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdbool.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}	t_philo;

struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	bool			someone_died;
	bool			stop;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	pthread_mutex_t	death;
	pthread_mutex_t	meals;
	t_philo			*philos;
	pthread_t		monitor;
};

int			parse_args(int argc, char **argv, t_data *data);
int			init_data(t_data *data);
void		*philo_routine(void *arg);
void		*monitor_routine(void *arg);
long long	get_time(void);
void		print_status(t_philo *philo, char *status);
void		cleanup(t_data *data);
int			ft_atoi(const char *str);
void		smart_sleep(long long time, t_data *data);
int			is_numeric(const char *str);
void		order_forks(t_philo *philo, pthread_mutex_t **first,
				pthread_mutex_t **second);
void		release_forks(t_philo *philo);

#endif
