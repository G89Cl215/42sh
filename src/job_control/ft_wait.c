/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wait_foreground.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgouedar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/16 20:40:42 by tgouedar          #+#    #+#             */
/*   Updated: 2020/03/05 18:48:25 by tgouedar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "jcont.h"
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

extern int		g_retval;

static int		ft_afterwait(t_job *job)
{
	int			ret_status;

	if (WIFSTOPPED(job->status))
	{
		ret_status = job->status;
		g_retval = WHARD_EXIT | WSTOPSIG(ret_status);
	}
	else
	{
		ret_status = ((t_process*)(job->process->content))->status;
		if (WIFEXITED(ret_status))
			g_retval = WEXITSTATUS(ret_status);
		else
			g_retval = WHARD_EXIT | WTERMSIG(ret_status);
		ft_pop_job(job->nbr);
	}
	return (ret_status);
}

int				ft_wait_foreground(t_job *job)
{
	sigset_t	wakeup_sig;

	tcsetpgrp(STDIN_FILENO, job->pgid);
	sigfillset(&wakeup_sig);
	sigdelset(&wakeup_sig, SIGCHLD);
	while (job->status & RUNNING)
		sigsuspend(&wakeup_sig);
	tcsetpgrp(STDIN_FILENO, getpid());
	return (ft_afterwait(job));
}

int				ft_wait_background(t_job *job)
{
	sigset_t	wakeup_sig;

	sigfillset(&wakeup_sig);
	sigdelset(&wakeup_sig, SIGCHLD);
	while (job->status & RUNNING)
		sigsuspend(&wakeup_sig);
	return (ft_afterwait(job));
}
