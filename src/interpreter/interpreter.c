/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bprunevi <bprunevi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 12:13:59 by bprunevi          #+#    #+#             */
/*   Updated: 2019/12/18 13:08:29 by tgouedar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "builtins.h"
#include "parser.h"
#include "jcont.h"

#define STDIN 0
#define STDOUT 1
#define STDERR 2

extern char		**environ;
int				g_fd[3] = {STDIN, STDOUT, STDERR};
int				g_fclose = -1;

int				i_comp_list(t_elem left, t_elem right)
{
	left.v->f(left.v->left, left.v->right);
	ft_launch_job("plop", FOREGROUND);
	right.v->f(right.v->left, right.v->right);
	return (0);
}

int				i_pipe_sequence(t_elem left, t_elem right)
{
	int pipe_fd[2];
	int bckp;

	pipe(pipe_fd);
	ft_dprintf(2, "Create pipe with {%d->%d}\n", pipe_fd[0], pipe_fd[1]);
	//valeurs de g_fd et g_fclose : 0, 1, 2 | -1 
	bckp = g_fd[1];
	g_fd[1] = pipe_fd[1];
	g_fclose = pipe_fd[0];
	left.v->f(left.v->left, left.v->right);// 0, P0, 2 | P1
	g_fd[1] = bckp;
	g_fd[0] = pipe_fd[0];
	ft_dprintf(2, "pipe_seq closing {%d}\n", pipe_fd[1]);
	close(pipe_fd[1]);
	g_fclose = -1;
	right.v->f(right.v->left, right.v->right);// P1, 1, 2 | -1
	close(pipe_fd[0]);
	return (-1);
}

int				i_execnode(t_elem left, t_elem right)
{
	if (left.v)
		left.v->f(left.v->left, left.v->right);
	right.v->f(right.v->left, right.v->right);
	return(0);
}

int				i_simple_builtin(t_elem left, t_elem right)
{
	char		**save_env;
	int			save_stdfd[3];

	save_env = NULL;
	ft_save_term_fd(g_fd, save_stdfd);
	ft_stdredir(g_fd);
	if ((left.v))
	{
		save_env = environ;
		environ = ft_tabcpy(environ);
		left.v->f(left.v->left, left.v->right);
	}
	right.v->f(right.v->left, right.v->right);
	if ((save_env))
	{
		ft_tabdel(&environ);
		environ = save_env;
	}
	ft_stdredir(save_stdfd);
	return (0);
}

int				i_simple_command(t_elem left, t_elem right)
{
	t_node		execnode;

	if (is_a_builtin(right.v->left.c))
		return (i_simple_builtin(left, right));
	execnode.left = left;
	execnode.right = right;
	execnode.f = i_execnode;
	ft_add_process(execnode, g_fd, g_fclose);
	return (0);
}
