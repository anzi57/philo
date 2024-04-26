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

# define ERROR 1

# define PENDING 0
# define DEFAULT 1
# define PHILO_DEAD 2
# define ALL_PHILOS_EATEN 3
# define TERMINATE 4

typedef struct s_shared
{
	int	philo_max;
	long	time_die;
	long	time_eat;
	long	time_sleep;
	long	start_time;
	int	eat_max;
	int	flag;
	bool	start_flag;
	int	full_philos_nbr;
	int	dead_philo_nbr;
	int	dead_philo_time;
	pthread_mutex_t	start_mutex;
	pthread_mutex_t	shared_mutex;
}	t_shared;

typedef struct s_philo
{
	int	philo_nbr;
	long	time_last_ate;
	int	eat_count;
	t_shared	*shared_args;
	pthread_mutex_t	local_mutex;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
}	t_philo;


//	main.c
void	create_and_join_threads(t_philo *philo_arr, t_shared *shared_args);
void	peform_cleanup(t_philo *philo_arr, t_shared *shared_args);

//	track_philos_routine.c
void	wait_sync_start(t_shared *shared_args);
void	*one_philo_dead(t_philo **philo_arr, int index);
void	*all_philos_eaten(t_philo **philo_arr);
void	set_philos_initial_time_last_ate(t_philo **philo_arr);
void	*track_philos_routine(void *arg);

//	philo_routine.c
void	*philo_routine(void *arg);

//	philo_actions.c
void	print_custom_message_and_set_flag(t_philo *philo);
int		printf_with_shared_mutex(t_philo *philo, const char *msg);
int		pick_up_forks(t_philo *philo);
void	release_forks(t_philo *philo);
int		eat(t_philo *philo);
int		philo_sleep(t_philo *philo);
int		think(t_philo *philo);

//	init_args.c
int		validate_args(int ac, char **av);
void	init_shared_args(t_shared *shared_args, int ac, char **av);
void	assign_forks_to_philo(t_philo *philo_arr, int i, int philo_max, pthread_mutex_t *fork_arr);
t_philo	*init_philo_arr(t_shared *shared_args);

//	helper_fns.c
void	pwrap(int func_rv, int func_type);
int		ft_atoi(const char *nptr);
long	get_time_ms(void);


#endif
