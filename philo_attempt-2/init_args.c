/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 09:55:13 by achak             #+#    #+#             */
/*   Updated: 2024/04/25 18:15:17 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	validate_args(int ac, char **av)
{
	int	i;

	i = -1;
	if (ac < 5)
	{
		write(STDERR_FILENO, "Too few arguments\n", 18);
		return (-1);
	}
	while (*++av && ++i < 6)
	{
		if (ft_atoi(*av) <= 0)
		{
			write(STDERR_FILENO, "Argument too small\n", 19);
			return (-1);
		}
	}
	return (0);
}

void	init_shared_args(t_shared *shared_args, int ac, char **av)
{
	shared_args->philo_max = ft_atoi(av[1]);
	shared_args->time_die = (long)ft_atoi(av[2]);
	shared_args->time_eat = (long)ft_atoi(av[3]);
	shared_args->time_sleep = (long)ft_atoi(av[4]);
	shared_args->start_time = 0;
	shared_args->eat_max = -1;
	shared_args->flag = PENDING;
	shared_args->full_philos_nbr = 0;
	pwrap(pthread_mutex_init(&shared_args->shared_mutex, NULL), 4);
	if (ac >= 6)
		shared_args->eat_max = ft_atoi(av[5]);
}

void	assign_forks_to_philo(t_philo *philo_arr, int i,
		int philo_max, pthread_mutex_t *fork_arr)
{
	philo_arr[i].left_fork = &fork_arr[i];
	if (i + 1 != philo_max)
		philo_arr[i].right_fork = &fork_arr[i + 1];
	else
		philo_arr[i].right_fork = &fork_arr[0];
}

t_philo	*init_philo_arr(t_shared *shared_args)
{
	int			i;
	pthread_mutex_t	*fork_arr;
	t_philo		*philo_arr;

	i = -1;
	fork_arr = malloc(sizeof(pthread_mutex_t) * shared_args->philo_max);
	philo_arr = malloc(sizeof(t_philo) * shared_args->philo_max);
	if (!fork_arr || !philo_arr)
		return (NULL);
	while (++i < shared_args->philo_max)
		pwrap(pthread_mutex_init(&fork_arr[i], NULL), 4);
	i = -1;
	while (++i < shared_args->philo_max)
	{
		philo_arr[i].philo_nbr = i + 1;
		philo_arr[i].eat_count = 0;
		philo_arr[i].time_last_ate = 0;
		philo_arr[i].shared_args = shared_args;
		assign_forks_to_philo(philo_arr, i,
			shared_args->philo_max, fork_arr);
	}
	return (philo_arr);
}
