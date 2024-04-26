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

void	create_and_join_threads(t_philo *philo_arr, t_shared *shared_args)
{
	int		i;
	pthread_t	*thread_arr;
	
	i = 1;
	thread_arr = malloc(sizeof(pthread_t) * (shared_args->philo_max + 2));
	if (!thread_arr)
	{
		perform_cleanup(philo_arr, shared_args);
		write(STDERR_FILENO, "thread_arr malloc\n", 18);
		return ;
	}
	pwrap(pthread_create(&thread_arr[0], NULL, &print_routine, &philo_arr), 1);
	pwrap(pthread_create(&thread_arr[1], NULL, &free_routine, shared_args), 1);
	while (++i < shared_args->philo_max + 2)
		pwrap(pthread_create(&thread_arr[i], NULL,
			&philo_routine, &philo_arr[i - 2]), 1);
	pwrap(pthread_mutex_lock(&shared_args->flag_mutex), 6);
	shared_args->loop_flag = 1;
	shared_args->start_time = get_time_ms();
	pwrap(pthread_mutex_unlock(&shared_args->flag_mutex), 7);
	i = -1;
	while (++i < shared_args->philo_max + 2)
		pwrap(pthread_join(thread_arr[i], NULL), 2);
//	printf("in create threads fn: %p\n", shared_args->global_queue);
//	printf("in create threads fn: %p\n", shared_args->print_queue);
//	printf("in create threads fn: %p\n", shared_args->free_queue);
//	printf("in create threads fn: %p\n", shared_args->tail);
//	printf("in create threads fn: %p\n", shared_args->dump);
	free_list(&shared_args->global_queue);
	//free_list(&shared_args->tail);
	free_list(&shared_args->dump);
	free_list(&shared_args->print_queue);
	free_list(&shared_args->free_queue);
	free(thread_arr);
}

int	main(int ac, char **av)
{
	t_shared	shared_args;
	t_philo		*philo_arr;

	if (validate_args(ac, av) == -1)
		return (1);
	init_shared_args(&shared_args, ac, av);
	philo_arr = init_philo_arr(&shared_args);
	if (!philo_arr)
		return (1);
//	int i = -1;
//	while (++i < shared_args.philo_max)
//	{
//		printf("philo_arr[%d].philo_nbr = %d\n", i,
//			philo_arr[i].philo_nbr);
//		printf("philo_arr[%d].eat_count = %d\n", i,
//			philo_arr[i].eat_count);
//		printf("philo_arr[%d].state = %d\n", i,
//			philo_arr[i].state);
//		printf("philo_arr[%d].time_last_ate = %ld\n", i,
//			philo_arr[i].time_last_ate);
//		printf("&philo_arr[%d].state_mutex = %p\n", i,
//			&philo_arr[i].state_mutex);
//		printf("philo_arr[%d].left_fork = %p\n", i,
//			philo_arr[i].left_fork);
//		printf("philo_arr[%d].right_fork = %p\n", i,
//			philo_arr[i].right_fork);
//		printf("philo_arr[%d].shared_args = %p\n", i,
//			philo_arr[i].shared_args);
//		printf("&philo_arr[%d].shared_args->flag_mutex = %p\n", i,
//			&philo_arr[i].shared_args->flag_mutex);
//	}
//	printf("in main: %p\n", shared_args.global_queue);
//	printf("in main: %p\n", shared_args.print_queue);
//	printf("in main: %p\n", shared_args.free_queue);
//	printf("in main: %p\n", shared_args.tail);
//	printf("in main: %p\n", shared_args.dump);
//	free_list(&shared_args.global_queue);
//	//free_list(&shared_args.tail);
//	free_list(&shared_args.dump);
//	free_list(&shared_args.print_queue);
//	free_list(&shared_args.free_queue);
	create_and_join_threads(philo_arr, &shared_args);
	perform_cleanup(philo_arr, &shared_args);
}
