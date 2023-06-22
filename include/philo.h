#ifndef PHILO_H
# define PHILO_H



# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>


typedef struct s_data
{
	int						nbr_philo;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						max_eat;
	long int				start_t;
	pthread_mutex_t			write_mutex;
	pthread_mutex_t			dead;
	pthread_mutex_t			time_eat;
	pthread_mutex_t			finish;
	int						nb_p_finish;
	int						stop;
}							t_data;

typedef struct s_philo
{
	int						id;
	pthread_t				thread_id;
	pthread_t				thread_death_id;
	pthread_mutex_t			*right_fork;
	pthread_mutex_t			left_fork;
	t_data					*table_data;
	long int				ms_eat;
	unsigned int			nb_eat;
	int						finish;
}							t_philo;

typedef struct s_table
{
	t_philo					*philo;
	t_data					data;
}							t_table;

/*
** functions
*/
void	ft_usleep(long int time_in_ms);
int check_death(t_philo *philo,  int i);
void *thread(void *data);
int				parse_args(char **argv, t_table *table);
int				init_argv(t_table *table);
int				ft_exit(char *str);
void	*is_dead(void	*data);

long long	timestamp(void);
int threading(t_table *table);
void	activity(t_philo *philo);
void	write_status(char *str, t_philo *philo);
void	sleep_think(t_philo *philo);




void			ft_putstr_fd(char *s, int fd);



int				ft_strlen(char *str);

#endif
