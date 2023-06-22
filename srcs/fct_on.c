#include "../include/philo.h"


void	ft_usleep(long int time_in_ms)
{
	long int	start_time;

	start_time = 0;
	start_time = timestamp();
	while ((timestamp() - start_time) < time_in_ms)
		usleep(time_in_ms / 10);
}

int check_death(t_philo *philo,  int i)
{
	pthread_mutex_lock(&philo->table_data->dead);
	if(i)
		philo->table_data->stop = i;
	if(philo->table_data->stop)
	{
		pthread_mutex_unlock(&philo->table_data->dead);
		return (1);
	}
	pthread_mutex_unlock(&philo->table_data->dead);
	return (0);
}



void	write_status(char *str, t_philo *philo)
{
	long int		time;

	time = -1;
	time = timestamp() - philo->table_data->start_t;
	if(time >= 0 && time <= 2147483647 && !check_death(philo, 0))
	{
		printf("%ld ", time);
		printf("Philo %d %s", philo->id, str);
	}
}

void sleep_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->table_data->write_mutex);
	write_status("is sleeping\n", philo);
	pthread_mutex_unlock(&philo->table_data->write_mutex);
	ft_usleep(philo->table_data->time_to_sleep);
	pthread_mutex_lock(&philo->table_data->write_mutex);
	write_status("is thinking\n", philo);
	pthread_mutex_unlock(&philo->table_data->write_mutex);
}


void	activity(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork);
	pthread_mutex_lock(&philo->table_data->write_mutex);
	write_status("has taken a fork\n", philo);
	pthread_mutex_unlock(&philo->table_data->write_mutex);
	if(!philo->right_fork)
	{
		ft_usleep(philo->table_data->time_to_die * 2);
		return ;
	}
	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(&philo->table_data->write_mutex);
	write_status("has taken a fork\n", philo);
	pthread_mutex_unlock(&philo->table_data->write_mutex);
	pthread_mutex_lock(&philo->table_data->write_mutex);
	write_status("is eating\n", philo);
	pthread_mutex_lock(&philo->table_data->time_eat);
	philo->ms_eat = timestamp();
	pthread_mutex_unlock(&philo->table_data->time_eat);
	pthread_mutex_unlock(&philo->table_data->write_mutex);
	ft_usleep(philo->table_data->time_to_eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(&philo->left_fork);
	sleep_think(philo);
}
/*

void *is_dead(void *data)
{

	t_philo *philo;

	philo = (t_philo *) data;
	ft_usleep(philo->table_data->time_to_die + 1);
	pthread_mutex_lock(&philo->table_data->time_eat);
	pthread_mutex_lock(&philo->table_data->finish);
	if(!check_death(philo, 0) && !philo->finish && (timestamp() - philo->ms_eat \
	>= (long)(philo->table_data->time_to_die)))
	{
		pthread_mutex_unlock(&philo->table_data->time_eat);
		pthread_mutex_unlock(&philo->table_data->finish);
		pthread_mutex_lock(&philo->table_data->write_mutex);
		write_status("died\n", philo);
		pthread_mutex_unlock(&philo->table_data->write_mutex);
		check_death(philo, 1);
	}
	pthread_mutex_unlock(&philo->table_data->time_eat);
	pthread_mutex_unlock(&philo->table_data->finish);
	return (NULL);

}
*/

void *is_dead(void *data)
{
	t_philo 		*philo;

	philo = (t_philo *) data;

    printf("Début du thread\n");

    printf("Fin du thread\n");

    return NULL;

}

void *thread(void *data)
{
	t_philo				*philo;
	philo = (t_philo *) data;
	if(philo->id % 2 == 0)
		ft_usleep(philo->table_data->time_to_eat / 10);
	while (!check_death(philo, 0))
	{
		printf("helloDEAD 1\n");
		if((pthread_create(&philo->thread_death_id, NULL, is_dead, data)) != 0)
		{
			printf("SORRY\n");
			return NULL;

		}
		printf("helloDEAD 2\n");
		activity(philo);
		pthread_detach(philo->thread_death_id);
		if((int)++philo->nb_eat == philo->table_data->max_eat)
		{
			pthread_mutex_lock(&philo->table_data->finish);
			philo->finish = 1;
			philo->table_data->nb_p_finish++;
			if(philo->table_data->nb_p_finish == philo->table_data->nbr_philo)
			{
				pthread_mutex_lock(&philo->table_data->finish);
				check_death(philo, 2);
			}
			pthread_mutex_unlock(&philo->table_data->finish);
			return (NULL);
		}
	}
	return (NULL);
}

int threading(t_table *table)
{
	int i;

	i = 0;
	while (i < table->data.nbr_philo)
	{
		table->philo[i].table_data = &table->data;

		if(pthread_create(&table->philo[i].thread_id, NULL, thread, &table->philo[i]) != 0)
		{
			return (ft_exit("Pthread did not return 0\n"));
		}
		i++;
	}

	return (1);
}
