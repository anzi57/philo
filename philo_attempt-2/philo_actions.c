/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 17:34:01 by achak             #+#    #+#             */
/*   Updated: 2024/04/25 19:30:39 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_custom_message_and_set_flag(t_philo *philo)
{
	if (philo->shared_args->flag == PHILO_DEAD)
	{
		printf("%ld philosopher %d died\n", philo->shared_args->dead_philo_time
			- philo->shared_args->start_time,
			philo->shared_args->dead_philo_nbr);
		philo->shared_args->flag = TERMINATE;
	}
	else if (philo->shared_args->flag == ALL_PHILOS_EATEN)
	{
//		printf("%ld all philosophers have eaten\n", get_time_ms()
//			- philo->shared_args->start_time);
		philo->shared_args->flag = TERMINATE;
	}
}

int	printf_with_shared_mutex(t_philo *philo, const char *msg)
{
	pwrap(pthread_mutex_lock(&philo->shared_args->shared_mutex), 6);
	if (philo->shared_args->flag == DEFAULT)
	{
//		printf("%ld philosopher %d %s\n", get_time_ms() - philo->shared_args->start_time,
//			philo->philo_nbr, msg);
		pwrap(pthread_mutex_unlock(&philo->shared_args->shared_mutex), 7);
	}
	else
	{
		print_custom_message_and_set_flag(philo);
		pwrap(pthread_mutex_unlock(&philo->shared_args->shared_mutex), 7);
		return (TERMINATE);
	}
	return (DEFAULT);
}

int	pick_up_forks(t_philo *philo)
{
	if (philo->philo_nbr % 2 == 0)
	{
		pwrap(pthread_mutex_lock(philo->right_fork), 6);
		if (printf_with_shared_mutex(philo, "has taken a fork") == TERMINATE)
		{
			pwrap(pthread_mutex_unlock(philo->right_fork), 7);
			return (TERMINATE);
		}
		pwrap(pthread_mutex_lock(philo->left_fork), 6);
		if (printf_with_shared_mutex(philo, "has taken a fork") == TERMINATE)
		{
			pwrap(pthread_mutex_unlock(philo->right_fork), 7);
			pwrap(pthread_mutex_unlock(philo->left_fork), 7);
			return (TERMINATE);
		}
	}
	else
	{
		pwrap(pthread_mutex_lock(philo->left_fork), 6);
		if (printf_with_shared_mutex(philo, "has taken a fork") == TERMINATE)
		{
			pwrap(pthread_mutex_unlock(philo->left_fork), 7);
			return (TERMINATE);
		}
		pwrap(pthread_mutex_lock(philo->right_fork), 6);
		if (printf_with_shared_mutex(philo, "has taken a fork") == TERMINATE)
		{
			pwrap(pthread_mutex_unlock(philo->left_fork), 7);
			pwrap(pthread_mutex_unlock(philo->right_fork), 7);
			return (TERMINATE);
		}
	}
	return (0);
}

void	release_forks(t_philo *philo)
{
	if (philo->philo_nbr % 2 == 0)
	{
		pwrap(pthread_mutex_unlock(philo->right_fork), 7);
		pwrap(pthread_mutex_unlock(philo->left_fork), 7);
	}
	else
	{
		pwrap(pthread_mutex_unlock(philo->left_fork), 7);
		pwrap(pthread_mutex_unlock(philo->right_fork), 7);
	}
}

int	eat(t_philo *philo)
{
	pwrap(pthread_mutex_lock(&philo->local_mutex), 6);
	philo->time_last_ate = get_time_ms();
	pwrap(pthread_mutex_unlock(&philo->local_mutex), 7);
	if (printf_with_shared_mutex(philo, "is eating") == TERMINATE)
		return (TERMINATE);
	usleep(philo->shared_args->time_eat * MICROSECS_PER_MS);
	release_forks(philo);
	pwrap(pthread_mutex_lock(&philo->local_mutex), 6);
	philo->eat_count++;
	if (philo->eat_count == philo->shared_args->eat_max)
	{
		pwrap(pthread_mutex_unlock(&philo->local_mutex), 7);
		return (TERMINATE);
	}
	pwrap(pthread_mutex_unlock(&philo->local_mutex), 7);
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	if (printf_with_shared_mutex(philo, "is sleeping") == TERMINATE)
		return (TERMINATE);
	usleep(philo->shared_args->time_sleep * MICROSECS_PER_MS);
	return (0);
}

int	think(t_philo *philo)
{
	if (printf_with_shared_mutex(philo, "is thinking") == TERMINATE)
		return (TERMINATE);
	return (0);
}
