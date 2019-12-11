/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitutions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baavril <baavril@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/06 20:52:32 by baavril           #+#    #+#             */
/*   Updated: 2019/11/09 11:47:27 by baavril          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansions.h"
#include "shell_variables.h"
#include "builtins.h"
#include "libft.h"

int		direct_exp(char **token)
{
	struct s_svar *tmp;

	tmp = g_svar;
	while (g_svar)
	{
		if (ft_strncmp(g_svar->key, *token + 1, ft_strlen(g_svar->key) - 1) == 0)
		{
			ft_strdel(token);
			*token = ft_strdup(g_svar->value);
			g_svar = tmp;
			return (SUCCESS);
		}
		g_svar = g_svar->next;
	}
	g_svar = tmp;
	return (ERROR);
}

int		simple_exp(char **token)
{
	struct s_svar *tmp;

	tmp = g_svar;
	while (g_svar)
	{
		if (ft_strncmp(g_svar->key, *token + 2, ft_strlen(g_svar->key) - 1) == 0)
		{
			ft_strdel(token);
			*token = ft_strdup(g_svar->value);
			g_svar = tmp;
			return (SUCCESS);
		}
		g_svar = g_svar->next;
	}
	g_svar = tmp;
	return (ERROR);

}

char *ft_strcdup(char *token, char c)
{
	int i;
	char *ret;

	i = 0;
	ret = NULL;
	while (token[i] != c)
		++i;
	if (!(ret = (char*)malloc(sizeof(char) * (i + 1))))
		return (0);
	i = 0;
	while (token[i] != c)
	{
		ret[i] = token[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

int		why_exp(char **token)
{
	char *var;
	char *word;
	struct s_svar *tmp;

	tmp = g_svar;
	word = ft_strcdup(ft_strchr(*token, WHY) + 1, CL_BRACE);
	var = ft_strcdup(*token + 2, COLON);
	while (g_svar)
	{
		if (ft_strncmp(g_svar->key, *token + 2, ft_strlen(g_svar->key) - 1) == 0)
		{
			ft_strdel(token);
			if (g_svar->value)
				*token = ft_strdup(g_svar->value);
			else
			{
				*token = ft_strdup(EMPTY_STR);
				ft_printf("42sh : %s: %s\n", var, word);
			}
			ft_strdel(&var);
			ft_strdel(&word);
			g_svar = tmp;
			return (SUCCESS);
		}
		g_svar = g_svar->next;
	}
	ft_strdel(token);
	*token = ft_strdup(EMPTY_STR);
	ft_printf("42sh : %s: %s\n", var, word);
	ft_strdel(&var);
	ft_strdel(&word);
	g_svar = tmp;
	return (ERROR);
}

int		plus_exp(char **token)
{
	(void)token;

	ft_printf("plus success");
	return (SUCCESS);
}

int		dash_exp(char **token)
{
	char *keep;
	struct s_svar *tmp;

	tmp = g_svar;
	keep = ft_strchr(*token, DASH) + 1;
	while (g_svar)
	{
		if (ft_strncmp(g_svar->key, *token + 2, ft_strlen(g_svar->key) - 1) == 0)
		{
			ft_strdel(token);
			if (g_svar->value)
				*token = ft_strdup(g_svar->value);
			else
				*token = ft_strndup(keep, ft_strlen(keep) - 1);
			g_svar = tmp;
			return (SUCCESS);
		}
		g_svar = g_svar->next;
	}
	ft_strdel(token);
	*token = ft_strndup(keep, ft_strlen(keep) - 1);
	g_svar = tmp;
	return (ERROR);
}

char	*setasvar(char *token)
{
	char *ret;
	char *var;
	char *word;

	var = ft_strcdup(token + 2, COLON);
	word = ft_strcdup(ft_strchr(token, EQUAL), CL_BRACE);
	ret = ft_strjoin(var, word);
	ft_strdel(&word);
	ft_strdel(&var);
	return (ret);
}

int		equal_exp(char **token)
{
	char *keep;
	char *nod;
	struct s_svar *tmp;

	tmp = g_svar;
	nod = setasvar(*token);
	keep = ft_strchr(*token, EQUAL) + 1;
	while (g_svar)
	{
		if (ft_strncmp(g_svar->key, *token + 2, ft_strlen(g_svar->key) - 1) == 0)
		{
			ft_strdel(token);
			if (g_svar->value)
				*token = ft_strdup(g_svar->value);
			else
			{
				*token = ft_strdup(keep);
				g_svar = tmp;
				if (checkvarlst(nod))
					listadd_back(newnodshell(nod, 0));
			}
			g_svar = tmp;
			cmd_set(0, NULL);
			return (SUCCESS);
		}
		g_svar = g_svar->next;
	}
	ft_strdel(token);
	*token = ft_strndup(keep, ft_strlen(keep) - 1);
	g_svar = tmp;
	listadd_back(newnodshell(nod, 0));
	cmd_set(0, NULL);
	return (ERROR);
}

int		bsharp_exp(char **token)
{
	struct s_svar *tmp;

	tmp = g_svar;
	while (g_svar)
	{
		if (ft_strncmp(g_svar->key, *token + 3, ft_strlen(g_svar->key) - 1) == 0)
		{
			ft_strdel(token);
			*token = ft_itoa(ft_strlen(g_svar->value));
			g_svar = tmp;
			return (SUCCESS);
		}
		g_svar = g_svar->next;
	}
	g_svar = tmp;
	return (ERROR);
}

int		osharp_exp(char **token)
{
	(void)token;

	ft_printf("osharp success");
	return (SUCCESS);
}

int		dsharp_exp(char **token)
{
	(void)token;

	ft_printf("dsharp success");
	return (SUCCESS);
}

int		opercent_exp(char **token)
{
	(void)token;

	ft_printf("opercent success");
	return (SUCCESS);
}

int		dpercent_exp(char **token)
{
	(void)token;

	ft_printf("dpercent success");
	return (SUCCESS);
}
