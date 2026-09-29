/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mezahir <mezahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 21:25:57 by mezahir           #+#    #+#             */
/*   Updated: 2026/07/12 20:32:57 by mezahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->dongles_creat)
	{
		pthread_cond_destroy(&data->dongles[i].cond);
		pthread_mutex_destroy(&data->dongles[i].mutex);
		if (data->dongles[i].heap.arr)
			free(data->dongles[i].heap.arr);
		i++;
	}
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->threads_created)
	{
		pthread_join((data->coders[i]).thread, NULL);
		i++;
	}
	if (data->monitor_created)
		pthread_join(data->monitor, NULL);
	destroy_dongles(data);
	destroy_data_mutex(data);
	free(data->dongles);
	if (data->coders)
		free(data->coders);
}

void	destroy_data_mutex(t_data *data)
{
	pthread_mutex_destroy(&data->time);
	pthread_mutex_destroy(&data->stop);
	pthread_mutex_destroy(&data->print);
}
