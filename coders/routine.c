/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mezahir <mezahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 11:22:58 by mezahir           #+#    #+#             */
/*   Updated: 2026/07/22 21:38:25 by mezahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	queue(t_dongle *dongle, t_coder *coder)
{
	long	deadline;

	pthread_mutex_lock(&coder->data->time);
	deadline = coder->last_compile_start + coder->data->time_to_burnout;
	pthread_mutex_unlock(&coder->data->time);
	pthread_mutex_lock(&dongle->mutex);
	insert(deadline, &dongle->heap, coder);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
	return ;
}

int	take_dongle(t_dongle *dongle, t_coder *coder)
{
	struct timespec	ts;

	pthread_mutex_lock(&dongle->mutex);
	while (!is_stoped(coder->data))
	{
		if (dongle->heap.size == 0 || dongle->heap.arr[0].coder_id != coder->id
			|| dongle->in_use == 1)
		{
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
			continue ;
		}
		if (get_current_time()
			- dongle->time_last_used >= coder->data->dongle_cooldown)
		{
			dongle->in_use = 1;
			pthread_mutex_unlock(&dongle->mutex);
			return (0);
		}
		ts = time_spec(dongle->time_last_used + coder->data->dongle_cooldown);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
	}
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}

int	take_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		queue(coder->right_dongle, coder);
		if (take_dongle(coder->right_dongle, coder))
			return (1);
		ft_print(coder, "has taken a dongle");
		queue(coder->left_dongle, coder);
		if (take_dongle(coder->left_dongle, coder))
			return (1);
		ft_print(coder, "has taken a dongle");
	}
	else
	{
		queue(coder->left_dongle, coder);
		if (take_dongle(coder->left_dongle, coder))
			return (1);
		ft_print(coder, "has taken a dongle");
		queue(coder->right_dongle, coder);
		if (take_dongle(coder->right_dongle, coder))
			return (1);
		ft_print(coder, "has taken a dongle");
	}
	return (0);
}

int	compile(t_coder *coder)
{
	if (take_dongles(coder))
		return (1);
	pthread_mutex_lock(&(coder->data)->time);
	coder->last_compile_start = get_current_time();
	coder->compiles_count++;
	pthread_mutex_unlock(&coder->data->time);
	ft_print(coder, "is compiling");
	ft_usleep(coder->data->time_to_compile, coder->data);
	release_dongle(coder->right_dongle, coder->data, coder);
	release_dongle(coder->left_dongle, coder->data, coder);
	return (0);
}

void	*routine(void *thread)
{
	t_coder	*coder;

	coder = (t_coder *)thread;
	if (coder->data->number_of_coders == 1)
		return (one_coder(coder));
	while (!is_stoped(coder->data))
	{
		if (coder->compiles_count == coder->data->number_of_compiles_required)
			break ;
		if (compile(coder))
			break ;
		if (debugging(coder))
			break ;
		if (refactoring(coder))
			break ;
	}
	return (NULL);
}
