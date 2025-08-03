/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:25:32 by rsommer           #+#    #+#             */
/*   Updated: 2025/05/17 17:25:33 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_death_or_stop(t_philo *philo)
{
	int	status;

	pthread_mutex_lock(&philo->data->death);
	status = philo->data->someone_died || philo->data->stop;
	pthread_mutex_unlock(&philo->data->death);
	return (status);
}

static void	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (check_death_or_stop(philo))
		return ;
	if (philo->data->num_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	order_forks(philo, &first_fork, &second_fork);
	pthread_mutex_lock(first_fork);
	if (check_death_or_stop(philo))
	{
		pthread_mutex_unlock(first_fork);
		return ;
	}
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(second_fork);
	print_status(philo, "has taken a fork");
}

static void	eat(t_philo *philo)
{
	if (philo->data->num_philos == 1)
	{
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->data->death);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->data->death);
	smart_sleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_lock(&philo->data->meals);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->meals);
	release_forks(philo);
}

static int	run_philo_cycle(t_philo *philo)
{
	if (check_death_or_stop(philo) || (philo->data->must_eat != -1
			&& philo->meals_eaten >= philo->data->must_eat))
		return (1);
	take_forks(philo);
	if (check_death_or_stop(philo))
		return (1);
	if (philo->data->num_philos == 1)
		return (usleep(philo->data->time_to_die * 1000), 1);
	eat(philo);
	if (philo->data->must_eat != -1
		&& philo->meals_eaten >= philo->data->must_eat)
		return (1);
	print_status(philo, "is sleeping");
	smart_sleep(philo->data->time_to_sleep, philo->data);
	print_status(philo, "is thinking");
	if (philo->data->num_philos % 2 == 1)
		usleep((philo->data->time_to_eat + philo->data->time_to_sleep)
			* 1000 / 10);
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	int		delay;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0 && philo->data->num_philos > 1)
	{
		delay = philo->data->num_philos * 20;
		if (delay > 5000)
			usleep(5000);
		else
			usleep(delay);
	}
	while (!run_philo_cycle(philo))
		;
	return (NULL);
}
