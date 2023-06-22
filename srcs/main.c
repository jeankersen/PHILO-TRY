#include "../include/philo.h"

int	ft_exit(char *str)
{
	ft_putstr_fd("Error : ", 2);
	ft_putstr_fd(str, 2);
	//printf("%s\n", str);
	return (0);
}


int	str_only_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i]   > '9')
			return (1);
		i++;
	}
	return (0);
}

int	check_argv(int argc, char **argv)
{
	int	i;

	i = 1;

	if (argc != 5 && argc != 6)
	{
		return (0);
	}
	while (i < argc)
	{
		if (str_only_digit(argv[i]))
		{
			return (0);
		}
		i++;
	}
	return (1);
}

int	check_death2(t_table *table)
{
	pthread_mutex_lock(&table->data.dead);
	if (table->data.stop)
	{
		pthread_mutex_unlock(&table->data.dead);
		return (1);
	}
	pthread_mutex_unlock(&table->data.dead);
	return (0);
}

void	stop(t_table *table)
{
	int	i;

	i = -1;
	pthread_mutex_destroy(&table->data.write_mutex);
	i = -1;
	while (++i < table->data.nbr_philo)
		pthread_mutex_destroy(&table->philo[i].left_fork);
	free(table->philo);
}




int	main(int argc, char **argv)
{
	t_table		table;


	if (!check_argv(argc, argv) || !(parse_args(argv, &table)))
		return (ft_exit("Invalid Arguments\n"));
	table.philo = malloc(sizeof(t_philo) * table.data.nbr_philo);
	if (!table.philo)
		return (ft_exit("Malloc returned NULL\n"));
	//init_argv(&table)


	if (!init_argv(&table) || !threading(&table))
	{

		free(table.philo);
		return (0);
	}
	printf("oh non\n");
	stop(&table);


	//return (0);
}
