/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_monitor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mezahir <mezahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 11:22:00 by mezahir           #+#    #+#             */
/*   Updated: 2026/07/15 10:04:36 by mezahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	stop(t_data *data, t_coder *coder)
{
	data->stop_flag = 1;
	ft_print(coder, "burned out");
}

int	check_dead(t_data *data, t_coder *coder)
{
	int		i;
	long	last;
	int		count;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->time);
		last = (coder + i)->last_compile_start;
		count = (coder + i)->compiles_count;
		pthread_mutex_unlock(&data->time);
		if (get_current_time() - last >= data->time_to_burnout
			&& count < coder->data->number_of_compiles_required)
		{
			pthread_mutex_lock(&data->stop);
			if (data->stop_flag == 0)
				stop(data, coder + i);
			pthread_mutex_unlock(&data->stop);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_finished(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->time);
		if (data->coders[i].compiles_count < data->number_of_compiles_required)
		{
			pthread_mutex_unlock(&data->time);
			return (0);
		}
		pthread_mutex_unlock(&data->time);
		i++;
	}
	pthread_mutex_lock(&data->stop);
	if (data->stop_flag == 0)
		data->stop_flag = 1;
	pthread_mutex_unlock(&data->stop);
	return (1);
}

void	wake_waiters(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->dongles[i].mutex);
		pthread_cond_broadcast(&data->dongles[i].cond);
		pthread_mutex_unlock(&data->dongles[i].mutex);
		i++;
	}
	return ;
}

void	*coders_monitor(void *coder)
{
	t_coder	*coders;
	t_data	*general_data;

	coders = (t_coder *)coder;
	general_data = coders[0].data;
	while (1)
	{
		if (check_dead(general_data, coders))
		{
			wake_waiters(general_data);
			return (NULL);
		}
		if (check_finished(general_data))
		{
			wake_waiters(general_data);
			return (NULL);
		}
	}
}
