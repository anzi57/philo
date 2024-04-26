/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 16:05:13 by achak             #+#    #+#             */
/*   Updated: 2024/04/25 18:41:17 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	perform_cleanup(t_philo *philo_arr, t_shared *shared_args)
{
	int	i;

	i = -1;
	while (++i < shared_args->philo_max)
	{
		pwrap(pthread_mutex_destroy(&philo_arr[i].local_mutex), 5);
		pwrap(pthread_mutex_destroy(philo_arr[i].left_fork), 5);
	}
	pwrap(pthread_mutex_destroy(&shared_args->shared_mutex), 5);
	free(philo_arr[0].left_fork);
	free(philo_arr);
}

void	create_and_join_threads(t_philo *philo_arr, t_shared *shared_args)
{
	int		i;
	pthread_t	*thread_arr;
	
	i = 0;
	thread_arr = malloc(sizeof(pthread_t) * (shared_args->philo_max + 1));
	if (!thread_arr)
	{
		perform_cleanup(philo_arr, shared_args);
		write(STDERR_FILENO, "thread_arr malloc\n", 18);
		return ;
	}
	pwrap(pthread_create(&thread_arr[0], NULL, &track_philos_routine, &philo_arr), 1);
	while (++i < shared_args->philo_max + 1)
		pwrap(pthread_create(&thread_arr[i], NULL,
			&philo_routine, &philo_arr[i - 1]), 1);
	pwrap(pthread_mutex_lock(&shared_args->shared_mutex), 6);
	shared_args->flag = DEFAULT;
	shared_args->start_time = get_time_ms();
	pwrap(pthread_mutex_unlock(&shared_args->shared_mutex), 7);
	i = -1;
	while (++i < shared_args->philo_max + 1)
		pwrap(pthread_join(thread_arr[i], NULL), 2);
	free(thread_arr);
}

int	main(int ac, char **av)
{
	t_shared	shared_args;
	t_philo		*philo_arr;

	if (validate_args(ac, av) == -1)
		return (ERROR);
	init_shared_args(&shared_args, ac, av);
	philo_arr = init_philo_arr(&shared_args);
	if (!philo_arr)
		return (ERROR);
	create_and_join_threads(philo_arr, &shared_args);
	perform_cleanup(philo_arr, &shared_args);
}
