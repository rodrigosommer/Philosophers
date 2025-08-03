/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:24:50 by rsommer           #+#    #+#             */
/*   Updated: 2025/05/17 17:24:51 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>

static int	start_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread,
				NULL, philo_routine, &data->philos[i]))
		{
			data->stop = true;
			cleanup(data);
			printf("Error: thread creation failed\n");
			return (1);
		}
		i++;
	}
	return (0);
}

static int	start_monitor(t_data *data)
{
	if (pthread_create(&data->monitor, NULL, monitor_routine, data))
	{
		data->stop = true;
		cleanup(data);
		printf("Error: monitor thread creation failed\n");
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (argc < 5 || argc > 6)
	{
		printf("Error: wrong number of arguments\n");
		return (1);
	}
	if (parse_args(argc, argv, &data))
	{
		printf("Error: invalid arguments\n");
		return (1);
	}
	if (init_data(&data))
	{
		printf("Error: initialization failed\n");
		return (1);
	}
	if (start_threads(&data) || start_monitor(&data))
		return (1);
	pthread_join(data.monitor, NULL);
	cleanup(&data);
	return (0);
}
