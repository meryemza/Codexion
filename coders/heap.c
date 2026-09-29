/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mezahir <mezahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 11:21:51 by mezahir           #+#    #+#             */
/*   Updated: 2026/07/23 15:28:36 by mezahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	creat_heap(t_data *data, t_dongle *dongle)
{
	dongle->heap.size = 0;
	dongle->heap.size_max = data->number_of_coders;
	dongle->heap.arr = malloc(sizeof(t_waiters) * dongle->heap.size_max);
	if (!dongle->heap.arr)
		return (1);
	return (0);
}

void	heapify_up(t_heap *heap, int current, int use_edf)
{
	int	parent;

	if (current == 0)
		return ;
	while (current > 0)
	{
		parent = (current - 1) / 2;
		if (has_priority(&heap->arr[current], &heap->arr[parent], use_edf))
		{
			swap(&heap->arr[current], &heap->arr[parent]);
			current = parent;
		}
		else
			return ;
	}
	return ;
}

void	insert(long deadline, t_heap *heap, t_coder *coder)
{
	if (heap->size >= heap->size_max)
	{
		printf("Cannot insert: heap is full\n");
		return ;
	}
	heap->arr[heap->size].deadline = deadline;
	heap->arr[heap->size].request = get_current_time() - coder->data->start;
	heap->arr[heap->size].coder_id = coder->id;
	heapify_up(heap, heap->size, coder->data->use_edf);
	heap->size += 1;
	return ;
}

void	heapify_down(t_heap *heap, int index_parent, int use_edf)
{
	int	left;
	int	right;
	int	max_prio;

	max_prio = index_parent;
	left = index_parent * 2 + 1;
	right = index_parent * 2 + 2;
	if (left < heap->size && (has_priority(&heap->arr[left],
				&heap->arr[max_prio], use_edf)))
		max_prio = left;
	if (right < heap->size && (has_priority(&heap->arr[right],
				&heap->arr[max_prio], use_edf)))
		max_prio = right;
	if (max_prio != index_parent)
	{
		swap(&heap->arr[index_parent], &heap->arr[max_prio]);
		index_parent = max_prio;
		heapify_down(heap, index_parent, use_edf);
	}
	return ;
}

void	*pop_top(t_heap *heap, int use_edf, t_coder *coder)
{
	if (!heap || !heap->arr)
		return (NULL);
	if (heap->size == 0)
		return (NULL);
	if (heap->size == 1)
	{
		heap->size -= 1;
		return (NULL);
	}
	if (heap->arr[0].coder_id == coder->id)
	{
		heap->arr[0] = heap->arr[heap->size - 1];
		heap->size -= 1;
		heapify_down(heap, 0, use_edf);
	}
	else
	{
		heap->size -= 1;
		heapify_down(heap, 0, use_edf);
	}
	return (NULL);
}
