/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mezahir <mezahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 20:04:31 by mezahir           #+#    #+#             */
/*   Updated: 2026/07/15 11:58:46 by mezahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

struct timespec	time_spec(long time_wake)
{
	struct timespec	ts;

	ts.tv_sec = time_wake / 1000;
	ts.tv_nsec = (time_wake % 1000) * 1000000L;
	return (ts);
}

long	get_current_time(void)
{
	long			current_time;
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
	{
		printf("gettimeofday failed");
		return (-1);
	}
	current_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (current_time);
}

void	init_para(t_data *data, char *argv[])
{
	data->coders = NULL;
	data->threads_created = 0;
	data->monitor_created = 0;
	data->stop_flag = 0;
	data->dongles_creat = 0;
	data->time_to_burnout = atoi(argv[2]);
	data->time_to_compile = atoi(argv[3]);
	data->time_to_debug = atoi(argv[4]);
	data->time_to_refactor = atoi(argv[5]);
	data->number_of_compiles_required = atoi(argv[6]);
	data->dongle_cooldown = atoi(argv[7]);
	if (strcmp(argv[8], "edf") == 0)
		data->use_edf = 1;
	else
		data->use_edf = 0;
}

void	suit_init_coder(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		(data->coders[i]).id = i + 1;
		(data->coders[i]).compiles_count = 0;
		(data->coders[i]).data = data;
		(data->coders[i]).left_dongle = (data->dongles + i);
		if (i == 0)
			(data->coders[i]).right_dongle = (data->dongles
					+ (data->number_of_coders) - 1);
		else
			(data->coders[i]).right_dongle = (data->dongles + i - 1);
		data->coders[i].last_compile_start = data->start;
		i++;
	}
}

int	init_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->time, NULL) || pthread_mutex_init(&data->stop,
			NULL) || pthread_mutex_init(&data->print, NULL))
	{
		printf("ERROR initializing mutexes");
		return (1);
	}
	return (0);
}
