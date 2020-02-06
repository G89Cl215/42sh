/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgouedar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 17:14:44 by tgouedar          #+#    #+#             */
/*   Updated: 2020/01/23 18:09:59 by tgouedar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/stat.h>

#include "builtin_cd.h"
#include "ft_getopt.h"
#include "libft.h"
#include "error.h"
#include "path.h"

extern int		g_optind;
extern int		g_opterr;
extern int		g_optopt;

static int		ft_update_pwd(char *new_pwd)
{
	char	*old_pwd;
	char	pwd[PATH_MAX];

	if (chdir(new_pwd) == -1)
	{
		free(new_pwd);
		return (TARGET_NOT_FOUND);
	}
	if ((old_pwd = ft_getenv("PWD")))
	{
		ft_setenv("OLDPWD", old_pwd);
		free(old_pwd);
	}
	getcwd(pwd, PATH_MAX);
	ft_setenv("PWD", pwd);
	return (EXEC_SUCCESS);
}

static int		ft_get_path(char *rel_path, char **new_pwd)
{
	if (!(rel_path))
	{
		if (!(*new_pwd = ft_getenv("HOME")))
			return (UNSET_HOME);
	}
	else if (!(ft_strcmp(rel_path, "-")))
	{
		if (!(*new_pwd = ft_getenv("OLDPWD")))
			return (UNSET_OLDPWD);
	}
	else
		*new_pwd = ft_strdup(rel_path); // ft_memcheck
	return (EXEC_SUCCESS);
}

static int		ft_cd_exec(char *target, int opt_p)
{
	int		ret;
	char	*new_pwd;

	if ((ret = ft_get_path(target, &new_pwd)))
		return (ret);
	ft_get_abspath(&new_pwd);
	if (!opt_p && (ret = ft_simplify_path(&new_pwd)))
	{
		ft_strdel(&new_pwd);
		return (ret);
	}
//	if (!ft_have_acces_right(new_pwd))
		//Pemission denied/is file;
	ret = ft_update_pwd(new_pwd);
	if (target && !(ft_strcmp(target, "-")))
		ft_putendl(new_pwd);
	ft_strdel(&new_pwd);
	return (ret);
}

static int		ft_parse_cd_opt(int argc, char **argv, int *p)
{
	int		opt;

	*p = 0;
	g_opterr = 1;
	g_optind = RESET_OPTIND;
	while ((opt = ft_getopt(argc, argv, CD_OPT)) != -1)
	{
		if (opt == 'P')
			*p |= 1;
		else if (opt == '?')
		{
			ft_dprintf(STDERR_FILENO, "cd: usage: cd [-L|-P] [dir].\n");
			return (2);
		}
	}
	return (0);
}

int				cmd_cd(int ac, char **av)
{
	int		opt_p;
	int		ret;
	char	*var_value;

	if ((ret = ft_parse_cd_opt(ac, av, &opt_p)))
		return (ret);
	if ((ret = ft_cd_exec(av[g_optind], opt_p)) == EXEC_SUCCESS)
	{
		if (!av[g_optind] || !ft_strcmp("-", av[g_optind]))
			var_value = "cd";
		else
			var_value = av[g_optind];
		ft_setenv("_", var_value);
	}
	else
		ret = ft_pcderror(ret, av[g_optind]);
	return (ret);
}