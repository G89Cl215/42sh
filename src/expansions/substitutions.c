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
	ft_strdel(token);
	*token = ft_strdup(EMPTY_STR);
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
	ft_strdel(token);
	*token = ft_strdup(EMPTY_STR);
	g_svar = tmp;
	return (ERROR);
}

char *ft_strcdup(char *token, char c)
{
	int i;
	char *ret;

	i = 0;
	ret = NULL;
	if (!token || !*token || *token == c)
		return (EMPTY_STR);
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
	struct s_svar *tmp;

	tmp = g_svar;
	while (g_svar)
	{
		if (ft_strncmp(g_svar->key, *token + 2, ft_strlen(g_svar->key) - 1) == 0)
		{
			ft_strdel(token);
			if (g_svar->value)
				*token = ft_strdup(g_svar->value);
			else
				*token = ft_strdup(EMPTY_STR);
			g_svar = tmp;
			return (SUCCESS);
		}
		g_svar = g_svar->next;
	}
	ft_strdel(token);
	*token = ft_strdup(EMPTY_STR);
	g_svar = tmp;
	return (ERROR);
}

int		dash_exp(char **token)
{
	char *keep;
	struct s_svar *tmp;

	tmp = g_svar;
	keep = ft_strcdup(ft_strchr(*token, DASH) + 1, CL_BRACE);
	while (g_svar)
	{
		if (ft_strncmp(g_svar->key, *token + 2, ft_strlen(g_svar->key) - 1) == 0)
		{
			ft_strdel(token);
			if (g_svar->value)
				*token = ft_strdup(g_svar->value);
			else
				*token = keep;
			g_svar = tmp;
			return (SUCCESS);
		}
		g_svar = g_svar->next;
	}
	ft_strdel(token);
	*token = keep;
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

char *ft_alpharange(char c, char x)
{
	int i;
	int lim;
	char *ret;

	i = 0;
	ret = NULL;
	lim = (x - c) + 1;
	if (lim > 1)
	{
		if (!(ret = (char *)malloc(sizeof(char) * (lim + 1))))
			return (0);
		while (i < lim)
			ret[i++] = c++;
		ret[i] = '\0';
	}
	else
		ret = ft_strdup(EMPTY_STR);
	return (ret);
}

char	*ft_deploy(char *match)
{
	int		i;
	int		j;
	int		flag;
	int		dash;
	char	*keep;
	char	*range;

	i = 0;
	j = 0;
	flag = 0;
	dash = 0;
	keep = NULL;
	range = NULL;
	if (!(keep = (char*)malloc(sizeof(char) * (ft_strlen(match) + 1))))
		return(0);
	while (match[i] && match[i + 1])
	{
		flag = 0;
		if (match[i + 1] == DASH)
		{
			if (ft_isalpha(match[i]) && ft_isalpha(match[i + 2]))
			{
				range = (!dash) ? ft_alpharange(match[i], match[i + 2]) : ft_strjoin(range, ft_alpharange(match[i], match[i + 2]));
				dash += 1;
				flag = 1;
				i += 2;
			}
		}
		if (!flag)
			keep[j++] = match[i];
		++i;
	}
	keep[j] = '\0';
	if (range)
		keep = ft_strjoin(keep, range);
	return (keep);
}

int		ft_getbmatch(char *token, char *match, char delim)
{
	int i;

	i = 0;
	if (ft_isin(DASH, match))
		match = ft_deploy(match);
	while (match[i] && match[i] != delim)
	{
		if (*token == match[i])
			return (SUCCESS);
		++i;
	}
	return (ERROR);
}

char *ft_strwhy(char *token, char *match)
{
	int i;
	int len;
	int lim;
	char *ret;

	i = 0;
	lim = 0;
	ret = NULL;
	len = (int)ft_strlen(token) - (int)ft_strlen(match);
	if (len > 0)
	{
		if (!(ret = (char*)malloc(sizeof(char) * (len + 1))))
			return (NULL);
		lim = ft_strlen(match);
	}
	else
		return (token);
	while (i < len)
		ret[i++] = token[lim++];
	ret[i] = '\0';
	return (ret);
}

int	ft_strpchr(char *str, char c)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] - c == 0)
			return (i);
		i++;
	}
	return (0);
}

char	*ft_strmatch(char *token, char *match)
{
	int len;
	char *ret;

	ret = NULL;
	len = ft_strlen(match);
	if (ft_strncmp(token, match, len) == 0)
		ret = ft_strdupfm(token, match[len - 1]);
	else if (match[ft_strpchr(match, WHY)] == WHY)
		ret = ft_strwhy(token, &match[ft_strpchr(match, WHY)]);
	else if (len == 1 && match[len - 1] == STAR)
	{
		/*here */
	}
	else if (len > 1 && *match == OP_SQUAR
	&& ft_getbmatch(token, match + 1, CL_SQUAR) == SUCCESS)
		ret = ft_strdup(&token[1]);
	else
		ret = ft_strdup(token);
	return (ret);
}

int		osharp_exp(char **token)
{
	char *word;
	struct s_svar *tmp;

	tmp = g_svar;
	word = ft_strcdup(ft_strchr(*token, SHARP) + 1, CL_BRACE);
	while (g_svar)
	{
		if (ft_strncmp(g_svar->key, *token + 2, ft_strlen(g_svar->key) - 1) == 0)
		{
			ft_strdel(token);
			*token = ft_strmatch(g_svar->value, word);
			g_svar = tmp;
			return (SUCCESS);
		}
		g_svar = g_svar->next;
	}
	ft_strdel(token);
	*token = ft_strdup(EMPTY_STR);
	g_svar = tmp;
	return (ERROR);
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
