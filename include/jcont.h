/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jcont.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgouedar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 16:18:20 by tgouedar          #+#    #+#             */
/*   Updated: 2020/03/07 16:18:22 by tgouedar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JCONT_H
# define JCONT_H

# include <stdlib.h>
# include <sys/wait.h>
# include "parser.h"
# include "sys/types.h"
# include "libft.h"

# define MAX_STATE_LEN			23
# define JOBS_OPT				"+lp"
# define L_OPT					1
# define P_OPT					2

# define NO_CHECK				0
# define STOP_CHECK				1

# define WHARD_EXIT				0x80
# define WSTPED					0x1f
# define RETVAL					0xffff
# define RUNNING				0x10000
# define BACKGROUND				0x20000
# define FOREGROUND				0x0
# define FORK_SHELL				0x40000
# define MAJOR_FAILLURE			0x80000

# define RESERVED_INFD			10
# define RESERVED_OUTFD			11
# define RESERVED_ERRFD			12

typedef struct		s_process
{
	pid_t			pid;
	int				status;
}					t_process;

typedef struct		s_job
{
	t_list			*process;
	pid_t			pgid;
	int				status;
	char			*cmd;
	int				nbr;
}					t_job;

typedef struct		s_jcont
{
	t_list			*jobs;
	int				job_nbr;
	int				active_jobs[2];
}					t_jcont;

int					ft_get_nbr_pgid(int nbr);
t_job				*ft_get_job_nbr(int job_nbr);
t_job				*ft_get_job_pgid(pid_t pgid);

t_process			*ft_get_process_from_job(t_job *job, pid_t pid);
t_process			*ft_get_process_pid(pid_t pid);

void				ft_print_jobs(t_list *job_list, int opt);
void				ft_print_job(t_job *job, int opt);
void				ft_free_proc(void *content, size_t size);
void				ft_free_job(void *content, size_t size);
int					ft_free_jcont(int flag);
void				ft_set_prio(void);
int					ft_pop_job(int nbr);
void				ft_update_job_status(void);
void				ft_check_bgstatus(void);
int					ft_wait_background(t_job *job);
int					ft_wait_foreground(t_job *job);
int					ft_isready(t_job *job);

int					ft_add_process(t_elem left, t_elem right, int std_fd[3],
															int fd_to_close);
int					ft_launch_job(void);
t_job				*ft_add_job(void);

void				ft_save_term_fd(int save_fd[3]);
void				ft_stdredir(int std_fd[3]);

int					ft_resume_in_fg(t_job *job);
int					ft_resume_in_bg(t_job *job);

#endif
