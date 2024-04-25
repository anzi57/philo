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

int	pick_up_forks(t_philo *philo)
{
//	if (check_if_philo_dead(philo) == 1)
//		return (1);
	if (philo->philo_nbr % 2 == 0)
	{
		pwrap(pthread_mutex_lock(philo->right_fork), 6);
		enqueue(philo->shared_args, philo->philo_nbr, FORK);
		pwrap(pthread_mutex_lock(philo->left_fork), 6);
		enqueue(philo->shared_args, philo->philo_nbr, FORK);
	}
	else
	{
		pwrap(pthread_mutex_lock(philo->left_fork), 6);
		enqueue(philo->shared_args, philo->philo_nbr, FORK);
		pwrap(pthread_mutex_lock(philo->right_fork), 6);
		enqueue(philo->shared_args, philo->philo_nbr, FORK);
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

int	check_if_philo_dead(t_philo *philo)
{
//	pwrap(pthread_mutex_lock(&philo->shared_args->queue_mutex), 6);
//	printf("philo_nbr = %d\n", philo->philo_nbr);
//	printf("get_time_ms() = %ld\n", get_time_ms());
//	printf("philo->time_last_ate (%ld) + philo->shared_args->time_die (%ld) = %ld\n", philo->time_last_ate, philo->shared_args->time_die, philo->time_last_ate + philo->shared_args->time_die);
//	pwrap(pthread_mutex_unlock(&philo->shared_args->queue_mutex), 7);
	if (get_time_ms() > philo->time_last_ate + philo->shared_args->time_die)
	{
		enqueue(philo->shared_args, philo->philo_nbr, DEAD);
		return (1);
	}
	return (0);
}

int	eat(t_philo *philo)
{
//	if (get_time_ms() > philo->time_last_ate + philo->shared_args->time_die)
//	{
//		enqueue(philo->shared_args, philo->philo_nbr, DEAD);
//		release_forks(philo);
//		return (1);
//	}
	if (check_if_philo_dead(philo) == 1)
	{
		release_forks(philo);
		return (1);
	}
	philo->time_last_ate = get_time_ms();
	enqueue(philo->shared_args, philo->philo_nbr, EAT);
	if (philo->shared_args->time_die > philo_shared_args->time_eat)
		usleep(philo->shared_args->time_eat * MICROSECS_PER_MS);
	else
	{
		usleep(philo->shared_args->time_die * MICROSECS_PER_MS);
		release_forks(philo);
		enqueue(philo->shared_args, philo->philo_nbr, DEAD);
		return (1);
	}
	release_forks(philo);
	philo->eat_count++;
	if (philo->eat_count == philo->shared_args->eat_max)
	{
		enqueue(philo->shared_args, philo->philo_nbr, FULL);
		return (1);
	}
	return (0);
}

int	philo_sleep(t_philo *philo)
{
//	if (check_if_philo_dead(philo) == 1)
//		return (1);
	enqueue(philo->shared_args, philo->philo_nbr, SLEEP);
	usleep(philo->shared_args->time_sleep * MICROSECS_PER_MS);
	return (0);
}

int	think(t_philo *philo)
{
//	if (check_if_philo_dead(philo) == 1)
//		return (1);
	enqueue(philo->shared_args, philo->philo_nbr, THINK);
	return (0);
}
