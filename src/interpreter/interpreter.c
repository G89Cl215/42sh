/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bprunevi <bprunevi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 12:13:59 by bprunevi          #+#    #+#             */
/*   Updated: 2020/07/09 01:35:48 by tgouedar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "jcont.h"
#include "error.h"

extern int				g_retval;
extern int				g_fd[3];
extern int				g_fclose;
int						g_prefix;
int						g_mode;

int				i_execnode(t_elem left, t_elem right)
{
	g_prefix = 1;
	if (!left.v || left.v->f(left.v->left, left.v->right) != -1)
		right.v->f(right.v->left, right.v->right);
	if (g_mode & FORK_SHELL)
		ft_launch_job();
	return (ft_clean_exit(NULL, g_retval));
}

static void		i_simple_command_builtin_prefix(t_elem left, t_elem right)
{
	extern char		**environ;
	char			**save_env;

	save_env = environ;
	environ = ft_tabcpy(environ);
	if (!(g_retval = (left.v->f(left.v->left, left.v->right) == -1)))
		right.v->f(right.v->left, right.v->right);
	ft_tabdel(&environ);
	environ = save_env;
}

static int		i_simple_command_builtin(t_elem left, t_elem right)
{
	int				save_stdfd[3];

	g_prefix = 1;
	ft_save_term_fd(save_stdfd);
	ft_stdredir(g_fd);
	if (left.v)
		i_simple_command_builtin_prefix(left, right);
	else
		g_retval = right.v->f(right.v->left, right.v->right);
	ft_stdredir(save_stdfd);
	g_prefix = 0;
	return (g_retval);
}

int				i_simple_command(t_elem left, t_elem right)
{
	if (left.v)
		expand_args(left.v);
	expand_args(right.v);
	if (right.v->f == i_builtin)
		return (i_simple_command_builtin(left, right));
	else
		ft_add_process(left, right, g_fd, g_fclose);
	return (-1);
}
