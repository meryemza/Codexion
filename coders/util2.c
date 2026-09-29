/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mezahir <mezahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 15:54:07 by mezahir           #+#    #+#             */
/*   Updated: 2026/07/22 21:37:50 by mezahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap(t_waiters *coder1, t_waiters *coder2)
{
	t_waiters	tmp;

	tmp = *coder1;
	*coder1 = *coder2;
	*coder2 = tmp;
}

void	ft_usleep(long time, t_data *data)
{
	long	start;

	start = get_current_time();
	while (!is_stoped(data) && get_current_time() - start < time)
		usleep(500);
}

int	has_priority(t_waiters *w1, t_waiters *w2, int use_edf)
{
	if (use_edf)
	{
		if (w1->deadline != w2->deadline)
			return (w1->deadline < w2->deadline);
		return (w1->request < w2->request);
	}
	return (w1->request < w2->request);
}

void	release_dongle(t_dongle *dongle, t_data *data, t_coder *coder)
{
	pthread_mutex_lock(&dongle->mutex);
	pop_top(&dongle->heap, data->use_edf, coder);
	dongle->time_last_used = get_current_time();
	dongle->in_use = 0;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
	return ;
}

void	*one_coder(t_coder *coder)
{
	ft_print(coder, "has taken a dongle");
	ft_usleep(coder->data->time_to_burnout, coder->data);
	return (NULL);
}
