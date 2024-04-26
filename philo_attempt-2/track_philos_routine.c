#include "philo.h"

void	wait_sync_start(t_shared *shared_args)
{
	while (1)
	{
		pwrap(pthread_mutex_lock(&shared_args->start_mutex), 6);
		if (shared_args->start_flag == true)
		{
			pwrap(pthread_mutex_unlock(&shared_args->start_mutex), 7);
			return ;
		}
		pwrap(pthread_mutex_unlock(&shared_args->start_mutex), 7);
	}
}

void	*one_philo_dead(t_philo **philo_arr, int index)
{
	pwrap(pthread_mutex_lock(&((*philo_arr)->shared_args->shared_mutex)), 6);
	(*philo_arr)->shared_args->dead_philo_nbr = index + 1;
	(*philo_arr)->shared_args->dead_philo_time = get_time_ms();
	(*philo_arr)->shared_args->flag = PHILO_DEAD;
	pwrap(pthread_mutex_unlock(&((*philo_arr)->shared_args->shared_mutex)), 7);
	return (NULL);
}

void	*all_philos_eaten(t_philo **philo_arr)
{
	pwrap(pthread_mutex_lock(&((*philo_arr)->shared_args->shared_mutex)), 6);
	(*philo_arr)->shared_args->flag = ALL_PHILOS_EATEN;
	pwrap(pthread_mutex_unlock(&((*philo_arr)->shared_args->shared_mutex)), 7);
	return (NULL);
}

void	set_philos_initial_time_last_ate(t_philo **philo_arr)
{
	int	i;

	i = -1;
	while (++i < (*philo_arr)->shared_args->philo_max)
	{
		pwrap(pthread_mutex_lock(&((*philo_arr)[i]).local_mutex), 6);
		if ((*philo_arr)[i].time_last_ate == 0)
			(*philo_arr)[i].time_last_ate = get_time_ms();
		pwrap(pthread_mutex_unlock(&((*philo_arr)[i]).local_mutex), 7);
	}
}

void	*track_philos_routine(void *arg)
{
	int		i;
	int		count;
	t_philo	**philo_arr;

	philo_arr = (t_philo **)arg;
	wait_sync_start((*philo_arr)->shared_args);
	set_philos_initial_time_last_ate(philo_arr);
	while (1)
	{
		i = -1;
		count = 0;
		while (++i < (*philo_arr)->shared_args->philo_max)
		{
			//printf("i = %d\n", i);
			pwrap(pthread_mutex_lock(&((*philo_arr)[i]).local_mutex), 6);
			if (((*philo_arr)[i]).time_last_ate
				+ (*philo_arr)->shared_args->time_die < get_time_ms())
			{
//				printf("current time: get_time_ms() = %ld\n", get_time_ms());
//				printf("(*philo_arr)[%d].time_last_ate (%ld) + (*philo_arr)->shared_args->time_die (%ld) = %ld\n", i, (*philo_arr)[i].time_last_ate, (*philo_arr)->shared_args->time_die, (*philo_arr)[i].time_last_ate + (*philo_arr)->shared_args->time_die);
				pwrap(pthread_mutex_unlock(&((*philo_arr)[i]).local_mutex), 7);
				return (one_philo_dead(philo_arr, i));
			}
			else if (((*philo_arr)[i]).eat_count == (*philo_arr)->shared_args->eat_max)
				count++;
			pwrap(pthread_mutex_unlock(&((*philo_arr)[i]).local_mutex), 7);
		}
		if (count == (*philo_arr)->shared_args->philo_max)
			return (all_philos_eaten(philo_arr));
	}
	return (NULL);
}
