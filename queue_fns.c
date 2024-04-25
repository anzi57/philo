/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_fns.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 12:38:27 by achak             #+#    #+#             */
/*   Updated: 2024/04/25 19:01:35 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_flag_equal_num(t_shared *shared_args, int num)
{
	pwrap(pthread_mutex_lock(&shared_args->flag_mutex), 6);
	if (shared_args->loop_flag == num)
	{
		pwrap(pthread_mutex_unlock(&shared_args->flag_mutex), 7);
		return (1);
	}
	pwrap(pthread_mutex_unlock(&shared_args->flag_mutex), 7);
	return (0);
}

void	enqueue(t_shared *shared_args, int philo_nbr, t_state state)
{
	t_list	*new_node;

	pwrap(pthread_mutex_lock(&shared_args->queue_mutex), 6);
	new_node = malloc(sizeof(t_list));
	if (!new_node)
	{
		pwrap(pthread_mutex_unlock(&shared_args->queue_mutex), 7);
		return ;
	}
	if (!shared_args->global_queue)
		shared_args->global_queue = new_node;
	else if (shared_args->tail)
		shared_args->tail->next = new_node;
	new_node->philo_nbr = philo_nbr;
	new_node->timestamp = get_time_ms() - shared_args->start_time;
	new_node->state = state;
	new_node->next = NULL;
	shared_args->tail = new_node;
	pwrap(pthread_mutex_unlock(&shared_args->queue_mutex), 7);
}

/*
void	free_global_queue(t_shared *shared_args)
{
	t_list	*temp;

	pwrap(pthread_mutex_lock(&shared_args->queue_mutex), 6);
	if (shared_args->global_queue == NULL)
	{
		pwrap(pthread_mutex_unlock(&shared_args->queue_mutex), 7);
		return ;
	}
	while (shared_args->global_queue)
	{
		temp = shared_args->global_queue->next;
		free(shared_args->global_queue);
		shared_args->global_queue = temp;
	}
	shared_args->global_queue = NULL;
	pwrap(pthread_mutex_unlock(&shared_args->queue_mutex), 7);
}
*/

void	wait_sync_start(t_shared *shared_args)
{
	while (1)
	{
		pwrap(pthread_mutex_lock(&shared_args->flag_mutex), 6);
		if (shared_args->loop_flag == 1)
		{
			pwrap(pthread_mutex_unlock(&shared_args->flag_mutex), 7);
			return ;
		}
		pwrap(pthread_mutex_unlock(&shared_args->flag_mutex), 7);
	}
}

void	*one_philo_routine(t_philo *philo)
{
	pwrap(pthread_mutex_lock(philo->left_fork), 6);
	enqueue(philo->shared_args, philo->philo_nbr, FORK);
	while (1)
	{
		if (check_if_philo_dead(philo) == 1)
		{
		//	enqueue(philo->shared_args, philo->philo_nbr, DEAD);
//			printf("in philo: %p\n", philo->shared_args->global_queue);
//			printf("in philo: %p\n", philo->shared_args->tail);
//			printf("in philo: %p\n", philo->shared_args->dump);
//			printf("in philo: %p\n", philo->shared_args->print_queue);
//			printf("in philo: %p\n", philo->shared_args->free_queue);
			pwrap(pthread_mutex_unlock(philo->left_fork), 7);
			break ;
		}
	}
	return (NULL);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_shared	*shared_args;

	philo = (t_philo *)arg;
	shared_args = philo->shared_args;
	wait_sync_start(shared_args);
	philo->time_last_ate = get_time_ms();
	if (shared_args->philo_max == 1)
		return (one_philo_routine(philo));
	while (1)
	{
		if (check_flag_equal_num(shared_args, 2) == 1)
			break ;
		if (pick_up_forks(philo) == 1)
			break ;
		if (eat(philo) == 1)
			break ;
		if (philo_sleep(philo) == 1)
			break ;
		if (think(philo) == 1)
			break ;
	}
	return (NULL);
}
