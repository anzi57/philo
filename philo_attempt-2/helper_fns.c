/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_fns.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 10:57:09 by achak             #+#    #+#             */
/*   Updated: 2024/04/24 20:44:17 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pwrap(int func_rv, int func_type)
{
	if (func_rv != 0)
	{
		if (func_type == 1)
			write(STDERR_FILENO, "pthread_create\n", 15);
		else if (func_type == 2)
			write(STDERR_FILENO, "pthread_join\n", 13);
		else if (func_type == 3)
			write(STDERR_FILENO, "pthread_detach\n", 15);
		else if (func_type == 4)
			write(STDERR_FILENO, "pthread_mutex_init", 19);
		else if (func_type == 5)
			write(STDERR_FILENO, "pthread_mutex_destroy\n", 22);
		else if (func_type == 6)
			write(STDERR_FILENO, "pthread_mutex_lock\n", 19);
		else if (func_type == 7)
			write(STDERR_FILENO, "pthread_mutex_unlock\n", 21);
	}
}

int	ft_atoi(const char *nptr)
{
	int		check;
	long	result;

	check = 1;
	result = 0;
	while ((*nptr == 32 || (*nptr >= 9 && *nptr <= 13)) && *nptr)
		nptr++;
	if (*nptr == '-')
		check = -1;
	if (*nptr == '-' || *nptr == '+')
		nptr++;
	while (*nptr != '\0' && *nptr >= '0' && *nptr <= '9')
	{
		result = result * 10 + (*nptr - '0');
		nptr++;
	}
	return ((int)(result * check));
}

long	get_time_ms(void)
{
	long	time;

	time = 0;
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		write(STDERR_FILENO, "gettimeofday\n", 13);
	time = (((tv.tv_sec % SECS_PER_DAY) * MILISECS_PER_SEC)
		+ (tv.tv_usec / MILISECS_PER_SEC));
	return (time);
}

/*#include <stdio.h>

int	main(void)
{
	const char *nptr = "    	 	   +2147483647abcde90";

	printf("ft_atoi: %d\n", ft_atoi(nptr));
	printf("atoi: %d\n", atoi(nptr));
}*/
