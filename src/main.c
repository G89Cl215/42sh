/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthel <abarthel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 18:32:13 by abarthel          #+#    #+#             */
/*   Updated: 2019/12/16 19:37:20 by tgouedar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "libft.h"
#include "shell_variables.h"
#include "history.h"
#include "sig_handler.h"
#include "expansions.h"
#include "builtins.h"
#include "prompt.h"
#include "input.h"
#include "error.h"
#include "lexer.h"
#include "parser.h"
#include "jcont.h"
#include "synt.h"
#include "path.h"

int	g_retval;
char	g_pwd[] = {0};

static int	set_minimal_env(void)
{
	char	*tmp;
	int		shlvl;

	tmp = getcwd(NULL, 0);
	if (ft_setenv("PWD", tmp, 1))
		return (e_cannot_allocate_memory);
	ft_memdel((void**)&tmp);
	if (!(tmp = ft_getenv("SHLVL")))
	{
		if (ft_setenv("SHLVL", "1", 1))
			return (e_cannot_allocate_memory);
	}
	else
	{
		shlvl = ft_atoi(tmp) + 1;
		tmp = ft_itoa(shlvl);
		if (ft_setenv("SHLVL", tmp, 1))
			return (e_cannot_allocate_memory);
		ft_memdel((void**)&tmp);
	}
	if (PATH_MAX > 0)
	{
		tmp = ft_getenv("PWD");
		if (ft_strlen(tmp) <= PATH_MAX)
			ft_strcpy(g_pwd, ft_getenv("PWD"));
	}
	return (e_success);
}

void		ft_sigusr1_handler(int nbr, siginfo_t *siginfo, void *context)
{
	extern t_jcont		g_jcont;
	extern t_job		g_curjob;
	t_process			*process;

	(void)nbr;
	(void)context;
	ft_dprintf(2, "\nJE SUIS DANS L'ACTION et j'ai le pid: %i\n\n", siginfo->si_pid);
	if (!(process = ft_get_process_pid(siginfo->si_pid))
	&& !(process = ft_get_process_from_job(&g_curjob, siginfo->si_pid)))
		return ;
	process->ready = 1;
}

int		main(int argc, char **argv)
{
	extern char		**environ;
	char			*input;
	char			**args;
	int				status;

	ft_dprintf(2, "Return of tcsetpgrp: %i for self\n", tcsetpgrp(STDIN_FILENO, getpid()));
	signal(SIGCHLD, &ft_sigchld_handler);
	
	struct sigaction	action;
	
	ft_bzero(&action, sizeof(sigaction));
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = &ft_sigusr1_handler;
	sigaction(SIGUSR1, &action, NULL);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	(void)argc;
	args = NULL;
	copybuff = NULL;
	input = NULL;
	g_progname = argv[0];
	init_shellvars(environ);
	/* cmd_set(argc, argv); to show the list of internvars */
	if (!(history(INIT, NULL, NULL)))
		return (1);
	if (!(environ = ft_tabcpy(environ)))
	{
		psherror(e_cannot_allocate_memory, argv[0], e_cmd_type);
		return (1);
	}
	g_retval = e_success;
	if ((g_retval = set_minimal_env()))
	{
		psherror(g_retval, argv[0], e_cmd_type);
		ft_tabdel(&environ);
		return (1);
	}
	set_signals(0);
	while (!read_command(&input) || get_next_line(0, &input))
	{
		if (!(status = history(ADD_CMD, &input, NULL)))
		{
			psherror(e_cannot_allocate_memory, argv[0], e_cmd_type);
			return (1);
		}
		if (status != -1 && input[0])
		{
			lexer(&input);
			debug_parser(input);
			/* way to test builtins without waiting interpreter */
			args = ft_strsplit_whitespaces(input);
			if (ft_strcmp("export", args[0]) == 0)
				cmd_export(0, args);
			else if (ft_strcmp("unset", args[0]) == 0)
				cmd_unset(0, args);
			else if (ft_strcmp("set", args[0]) == 0)
				cmd_set(0, args);
			else if (ft_strcmp("exit", args[0]) == 0)
				exit(0);
			else
				expansions_management(args);
			continue ; 
			args = lexer(&input);
			ft_memdel((void**)&input);
//			if (!args)
				continue;
			status = synt(args);
			if (status != e_success)
			{
				g_retval = status;
				ft_tabdel(&args);
				continue;
			}
			g_retval = 0;
			ft_tabdel(&args);
		}
		else
			ft_memdel((void**)&input);
	}
	history(DELETE, NULL, NULL);
	ft_tabdel(&environ);
	ft_strdel(&copybuff);
	ft_free_bintable();
	return (0);
}
