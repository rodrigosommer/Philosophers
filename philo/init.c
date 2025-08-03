/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:25:12 by rsommer           #+#    #+#             */
/*   Updated: 2025/05/17 17:25:13 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	validate_args(char **argv, int argc, t_data *data)
{
	if (!is_numeric(argv[1]))
		return (1);
	if (!is_numeric(argv[2]))
		return (1);
	if (!is_numeric(argv[3]))
		return (1);
	if (!is_numeric(argv[4]))
		return (1);
	if (argc == 6 && !is_numeric(argv[5]))
		return (1);
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->must_eat = -1;
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	return (0);
}

int	parse_args(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
		return (1);
	if (validate_args(argv, argc, data))
		return (1);
	if (data->num_philos < 1 || data->num_philos > 200)
		return (1);
	if (data->time_to_die < 0 || data->time_to_eat < 0)
		return (1);
	if (data->time_to_sleep < 0)
		return (1);
	if (argc == 6 && data->must_eat < 0)
		return (1);
	return (0);
}

static int	init_resources(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->print, NULL))
		return (1);
	if (pthread_mutex_init(&data->death, NULL))
		return (1);
	if (pthread_mutex_init(&data->meals, NULL))
		return (1);
	return (0);
}

static void	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = data->start_time;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		data->philos[i].data = data;
		i++;
	}
}

int	init_data(t_data *data)
{
	data->someone_died = false;
	data->stop = false;
	data->start_time = get_time();
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->philos || !data->forks)
		return (1);
	if (init_resources(data))
		return (free(data->philos), free(data->forks), 1);
	init_philos(data);
	return (0);
}
