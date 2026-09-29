/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mezahir <mezahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 22:38:17 by mezahir           #+#    #+#             */
/*   Updated: 2026/07/23 15:57:59 by mezahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_waiters
{
	int				coder_id;
	long			deadline;
	long			request;
}					t_waiters;

typedef struct s_heap
{
	t_waiters		*arr;
	int				size;
	int				size_max;
}					t_heap;

typedef struct s_dongle
{
	t_heap			heap;
	long			time_last_used;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				in_use;
}					t_dongle;
typedef struct s_data
{
	int				use_edf;
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	int				dongles_creat;
	int				threads_created;
	int				monitor_created;
	pthread_t		monitor;
	t_dongle		*dongles;
	pthread_mutex_t	time;
	pthread_mutex_t	stop;
	pthread_mutex_t	print;
	struct s_coder	*coders;
	long			start;
	int				stop_flag;
}					t_data;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	int				compiles_count;
	long			last_compile_start;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_data			*data;
}					t_coder;

int					parsing(int argc, char *argv[]);
int					valide_number(char *arg);
int					parsing(int argc, char *argv[]);
int					init_dongles(t_data *general_data);
void				*coders_monitor(void *coder);
long				get_current_time(void);
void				ft_usleep(long time, t_data *data);
void				ft_print(t_coder *coder, char *str);
int					is_stoped(t_data *data);
int					check_finished(t_data *data);
void				*one_coder(t_coder *coder);
int					take_dongle(t_dongle *dongle, t_coder *coder);
int					check_dead(t_data *data, t_coder *coder);
void				*routine(void *thread);
int					creat_heap(t_data *data, t_dongle *dongle);
void				*pop_top(t_heap *heap, int use_edf, t_coder *coder);
void				swap(t_waiters *coder1, t_waiters *coder2);
int					has_priority(t_waiters *w1, t_waiters *w2, int use_edf);
void				cleanup(t_data *data);
void				free_coders(t_coder *coders);
void				init_para(t_data *data, char *argv[]);
int					init_mutexes(t_data *data);
int					refactoring(t_coder *coder);
int					debugging(t_coder *coder);
int					is_stoped(t_data *data);
struct timespec		time_spec(long time_wake);
void				suit_init_coder(t_data *data);
int					has_priority(t_waiters *w1, t_waiters *w2, int use_edf);
void				destroy_data_mutex(t_data *data);
int					init_valide_data(char *argv[], t_data *data);
void				insert(long deadline, t_heap *heap, t_coder *coder);
void				release_dongle(t_dongle *dongle, t_data *data,
						t_coder *coder);
#endif