/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 11:23:00 by achak             #+#    #+#             */
/*   Updated: 2024/04/25 17:59:26 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_list(t_list **list)
{
	t_list	*temp;

	if (!list || !*list)
		return ;
	while (*list)
	{
		temp = (*list)->next;
		free(*list);
		*list = temp;
	}
	*list = NULL;
}

void	move_dump_to_free_queue(t_shared *shared_args)
{
	if (shared_args->free_queue != NULL)
		free_list(&shared_args->free_queue);
	pwrap(pthread_mutex_lock(&shared_args->dump_mutex), 6);
	if (!shared_args->dump)
	{
		pwrap(pthread_mutex_unlock(&shared_args->dump_mutex), 7);
		return ;
	}
	shared_args->free_queue = shared_args->dump;
	shared_args->dump = NULL;
	pwrap(pthread_mutex_unlock(&shared_args->dump_mutex), 7);
}

void	*free_routine(void *arg)
{
	t_shared	*shared_args;

	shared_args = (t_shared *)arg;
	wait_sync_start(shared_args);
	while (1)
	{
		if (check_flag_equal_num(shared_args, 2) == 1)
		{
//			printf("in free routine: %p\n", shared_args->global_queue);
//			printf("in free routine: %p\n", shared_args->free_queue);
//			printf("in free routine: %p\n", shared_args->print_queue);
//			printf("in free routine: %p\n", shared_args->tail);
//			printf("in free routine: %p\n", shared_args->dump);
			break ;
		}
		usleep(50000);
		move_dump_to_free_queue(shared_args);
//		free_list(&shared_args->free_queue);
	}
	return (NULL);
}

