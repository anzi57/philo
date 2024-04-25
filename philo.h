/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 16:01:46 by achak             #+#    #+#             */
/*   Updated: 2024/04/25 19:03:44 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

# define SECS_PER_DAY 86400
# define MILISECS_PER_SEC 1000
# define MICROSECS_PER_MS 1000

typedef enum e_state
{
	FORK,
	EAT,
	SLEEP,
	THINK,
	FULL,
	DEAD
}	t_state;

typedef struct s_list
{
	int		philo_nbr;
	long		timestamp;
	t_state		state;
	struct s_list	*next;
}	t_list;

typedef struct s_shared
{
	int	philo_max;
	long	time_die;
	long	time_eat;
	long	time_sleep;
	long	start_time;
	int	eat_max;
	int	loop_flag;
	int	full_philos_nbr;
	t_list	*global_queue;
	t_list	*tail;
	t_list	*print_queue;
	t_list	*dump;
	t_list	*free_queue;
	pthread_mutex_t	queue_mutex;
	pthread_mutex_t	dump_mutex;
	pthread_mutex_t	flag_mutex;
}	t_shared;

typedef struct s_philo
{
	int	philo_nbr;
	int	eat_count;
	long	time_last_ate;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_shared	*shared_args;
}	t_philo;

//	main.c
void	create_and_join_threads(t_philo *philo_arr, t_shared *shared_args);

//	queue_fns.c
int		check_flag_equal_num(t_shared *shared_args, int num);
void	enqueue(t_shared *shared_args, int philo_nbr, t_state state);
void	free_dump(t_shared *shared_args);
//void	free_global_queue(t_shared *shared_args);
int		print_global_queue(t_shared *shared_args);
void	wait_sync_start(t_shared *shared_args);
void	*one_philo_routine(t_philo *routine);
void	*philo_routine(void *arg);

//	print_routine.c
//void	print_message(t_list *queue_node, long start_time);
void	print_message(t_list *queue_node);
void	shift_queue_to_print(t_shared *shared_args);
int		check_if_terminate(t_shared *shared_args, t_list *temp);
int		print_out_queue(t_shared *shared_args);
void	*print_routine(void *arg);
void	dump_whole_queue(t_shared *shared_args, t_list *print_tail);

//	free_routine.c
void	free_list(t_list **list);
void	move_dump_to_free_queue(t_shared *shared_args);
void	*free_routine(void *arg);

//	philo_actions.c
int		pick_up_forks(t_philo *philo);
void	release_forks(t_philo *philo);
int		check_if_philo_dead(t_philo *philo);
int		eat(t_philo *philo);
int		philo_sleep(t_philo *philo);
int		think(t_philo *philo);

//	init_args.c
int		validate_args(int ac, char **av);
void	init_shared_args(t_shared *shared_args, int ac, char **av);
void	assign_forks_to_philo(t_philo *philo_arr, int i, int philo_max,
		pthread_mutex_t *fork_arr);
t_philo	*init_philo_arr(t_shared *shared_args);

//	helper_fns.c
void	pwrap(int func_rv, int func_type);
int		ft_atoi(const char *nptr);
long	get_time_ms(void);
int		my_strncmp(char *s1, char *s2, size_t n);
void	perform_cleanup(t_philo *philo_arr, t_shared *shared_args);

#endif
