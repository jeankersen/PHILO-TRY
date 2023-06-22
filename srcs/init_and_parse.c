#include "../include/philo.h"

int ft_atoi(const char *str)
{
	int i;
	long sum;

	sum = 0;
	i = -1;
	while (str[++i])
		sum = (sum * 10) + (str[i] - 48);
	return (sum);
}


int parse_args(char **argv, t_table *table)
{
		table->data.nbr_philo = ft_atoi(argv[1]);
		table->data.time_to_die = ft_atoi(argv[2]);
		table->data.time_to_eat = ft_atoi(argv[3]);
		table->data.time_to_sleep = ft_atoi(argv[4]);
		table->data.max_eat = -1;
		if (argv[5])
			table->data.max_eat = ft_atoi(argv[5]);
		if (table->data.nbr_philo <= 0 || table->data.time_to_die <= 0 || table->data.time_to_eat <= 0 || table->data.time_to_sleep <= 0)
			return (0);
		return (1);
}

void init_mutex(t_table *table)
{
	pthread_mutex_init(&table->data.write_mutex, NULL);
	pthread_mutex_init(&table->data.dead, NULL);
	pthread_mutex_init(&table->data.time_eat, NULL);
	pthread_mutex_init(&table->data.finish, NULL);
}

int init_argv(t_table *table)
{
	int i;

	i = 0;
	table->data.start_t = timestamp();
	table->data.stop = 0;
	table->data.nb_p_finish = 0;
	init_mutex(table);
	while (i < table->data.nbr_philo)
	{
		table->philo[i].id = i + 1;
		table->philo[i].ms_eat = table->data.start_t;
		table->philo[i].nb_eat = 0;
		table->philo[i].finish = 0;
		table->philo[i].right_fork = NULL;
		pthread_mutex_init(&table->philo[i].left_fork, NULL);
		if (table->data.nbr_philo == 1)
			return (1);
		if (i == table->data.nbr_philo - 1)
			table->philo[i].right_fork = &table->philo[0].left_fork;
		else
			table->philo[i].right_fork = &table->philo[i + 1].left_fork;
		i++;
	}
	return (1);
}

void ft_putchar_fd(char c, int fd)
{
	if (fd >= 0)
		write(fd, &c, 1);
}

void ft_putstr_fd(char *s, int fd)
{
	if (fd >= 0)
	{
		while (s && *s)
		{
			write(fd, &*s, 1);
			s++;
		}
	}
}


long long	timestamp(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}
