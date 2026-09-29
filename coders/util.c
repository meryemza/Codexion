/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mezahir <mezahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 11:20:35 by mezahir           #+#    #+#             */
/*   Updated: 2026/07/16 12:07:58 by mezahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	debugging(t_coder *coder)
{
	if (!is_stoped(coder->data))
	{
		ft_print(coder, "is debugging");
		ft_usleep(coder->data->time_to_debug, coder->data);
		return (0);
	}
	return (1);
}

int	refactoring(t_coder *coder)
{
	if (!is_stoped(coder->data))
	{
		ft_print(coder, "is refactoring");
		ft_usleep(coder->data->time_to_refactor, coder->data);
		return (0);
	}
	return (1);
}

int	is_stoped(t_data *data)
{
	int	flag;

	pthread_mutex_lock(&data->stop);
	flag = data->stop_flag;
	pthread_mutex_unlock(&data->stop);
	return (flag);
}

void	ft_print(t_coder *coder, char *str)
{
	pthread_mutex_lock(&(coder->data)->print);
	printf("%zu %d %s\n", get_current_time() - coder->data->start, coder->id,
		str);
	pthread_mutex_unlock(&(coder->data)->print);
}
