#include "philo.h"

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_sync_start(philo->shared_args);
//	pwrap(pthread_mutex_lock(&philo->local_mutex), 6);
//	philo->time_last_ate = get_time_ms();
//	pwrap(pthread_mutex_unlock(&philo->local_mutex), 7);
	while (1)
	{
		if (pick_up_forks(philo) == TERMINATE)
			break ;
		if (eat(philo) == TERMINATE)
			break ;
		if (philo_sleep(philo) == TERMINATE)
			break ;
		if (think(philo) == TERMINATE)
			break ;
	}
	return (NULL);
}
