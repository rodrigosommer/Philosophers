/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:25:54 by rsommer           #+#    #+#             */
/*   Updated: 2025/05/17 17:25:55 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_single_philo(t_data *data)
{
	long long	now;
	long long	last_meal;

	if (data->num_philos != 1)
		return (0);
	pthread_mutex_lock(&data->death);
	now = get_time();
	last_meal = data->philos[0].last_meal_time;
	if (now - last_meal >= data->time_to_die)
	{
		pthread_mutex_unlock(&data->death);
		pthread_mutex_lock(&data->print);
		printf("%lld %d died\n", now - data->start_time, data->philos[0].id);
		pthread_mutex_unlock(&data->print);
		pthread_mutex_lock(&data->death);
		data->someone_died = true;
		pthread_mutex_unlock(&data->death);
		return (1);
	}
	pthread_mutex_unlock(&data->death);
	return (0);
}

static int	check_philo_death(t_data *data, int i, long long now)
{
	long long	last_meal;

	last_meal = data->philos[i].last_meal_time;
	if (now - last_meal >= data->time_to_die)
	{
		pthread_mutex_unlock(&data->death);
		pthread_mutex_lock(&data->print);
		printf("%lld %d died\n", now - data->start_time, data->philos[i].id);
		pthread_mutex_unlock(&data->print);
		pthread_mutex_lock(&data->death);
		data->someone_died = true;
		pthread_mutex_unlock(&data->death);
		return (1);
	}
	return (0);
}

static int	check_all_philos(t_data *data, int *all_ate)
{
	int			i;
	long long	now;

	i = -1;
	while (++i < data->num_philos)
	{
		pthread_mutex_lock(&data->death);
		now = get_time();
		if (check_philo_death(data, i, now))
			return (1);
		pthread_mutex_unlock(&data->death);
		pthread_mutex_lock(&data->meals);
		if (data->must_eat != -1
			&& data->philos[i].meals_eaten < data->must_eat)
			*all_ate = 0;
		pthread_mutex_unlock(&data->meals);
	}
	return (0);
}

static void	handle_all_ate(t_data *data)
{
	pthread_mutex_lock(&data->death);
	data->someone_died = true;
	pthread_mutex_unlock(&data->death);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		all_ate;

	data = (t_data *)arg;
	while (!data->stop)
	{
		all_ate = 1;
		if (check_single_philo(data))
			return (NULL);
		if (check_all_philos(data, &all_ate))
			return (NULL);
		if (all_ate && data->must_eat != -1)
		{
			handle_all_ate(data);
			return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}
