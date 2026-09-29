/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mezahir <mezahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 11:22:29 by mezahir           #+#    #+#             */
/*   Updated: 2026/07/12 16:42:42 by mezahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	creat_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		if (pthread_create(&(data->coders[i]).thread, NULL, routine,
				&data->coders[i]))
		{
			printf("ERROR creating thread for coder\n");
			return (1);
		}
		data->threads_created += 1;
		i++;
	}
	if (pthread_create(&data->monitor, NULL, coders_monitor, data->coders))
	{
		printf("ERROR creating thread for monitor\n");
		return (1);
	}
	data->monitor_created = 1;
	return (0);
}

int	init_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		if (pthread_mutex_init(&data->dongles[i].mutex, NULL)
			|| pthread_cond_init(&data->dongles[i].cond, NULL))
			return (1);
		data->dongles_creat += 1;
		(data->dongles[i]).time_last_used = 0;
		(data->dongles[i]).in_use = 0;
		if (creat_heap(data, &data->dongles[i]))
			return (1);
		i++;
	}
	return (0);
}

int	init_coders(t_data *data)
{
	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!(data->coders))
		return (1);
	suit_init_coder(data);
	return (0);
}

int	init_data(t_data *data, char *argv[])
{
	data->number_of_coders = atoi(argv[1]);
	init_para(data, argv);
	data->start = get_current_time();
	if (init_mutexes(data))
		return (1);
	if (sizeof(t_dongle) * data->number_of_coders > INT_MAX)
	{
		destroy_data_mutex(data);
		return (1);
	}
	data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
	if ((!data->dongles))
	{
		destroy_data_mutex(data);
		return (1);
	}
	return (0);
}

int	init_valide_data(char *argv[], t_data *data)
{
	if (init_data(data, argv))
		return (1);
	if (init_dongles(data))
	{
		cleanup(data);
		return (1);
	}
	if (init_coders(data))
	{
		cleanup(data);
		return (1);
	}
	creat_threads(data);
	cleanup(data);
	return (0);
}
