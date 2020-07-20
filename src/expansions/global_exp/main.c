/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baavril <baavril@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/07 16:40:47 by baavril           #+#    #+#             */
/*   Updated: 2020/07/20 16:37:52 by tgouedar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "shell_variables.h"
#include "expansions.h"
#include "libft.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>


static void		free_quoted_token_lst(struct s_quoted *quoted_tokens)
{
	struct s_quoted *tmp;

	while (quoted_tokens)
	{
		ft_strdel(&(quoted_tokens->token));
		tmp = quoted_tokens->next;
		free(quoted_tokens);
		quoted_tokens = tmp;
	}
}

static int		quoted_linker(struct s_quoted *voyager, char **tokens,
													char **splitok, int flag)
{
	char			*tmp_to_free;
	char			*tmp_to_free_2;

	tmp_to_free = ft_strdup(*tokens);
	ft_strdel(&(*tokens));
	tmp_to_free_2 = expansions_management(splitok, voyager->expand, flag);
	*tokens = ft_strjoin(tmp_to_free, tmp_to_free_2);
	ft_strdel(&tmp_to_free);
	ft_strdel(&tmp_to_free_2);
	return (0);
}

static int		expansions_quoted_treatment(char **tokens,
													char **splitok, int flag)
{
	t_quoted		*quoted_tokens;
	t_quoted		*voyager;

	quoted_tokens = token_quotes_generator(*tokens);
	voyager = quoted_tokens;
	ft_strdel(tokens);
	while (voyager)
	{
		if (!(splitok = ft_expsplit(voyager->token)))
			return (ERROR);
		if (*tokens == NULL)
			*tokens = expansions_management(splitok, voyager->expand, flag);
		else
			quoted_linker(voyager, tokens, splitok, flag);
		voyager = voyager->next;
	}
	free_quoted_token_lst(quoted_tokens);
	return (0);
}

/*
** The flag int value in the functions of this file is there to
** single out the case of heredoc tokens (since they are unquoted, unslashed)
** but variables are not expanded
*/

int				expansions_treatment(char **tokens, int flag)
{
	char	**splitok;

	splitok = NULL;
	if (!flag && **tokens == TILDE)
		tilde_exp(tokens);
	if (*tokens && (ft_isin(DQUOTES, *tokens) || ft_isin(SQUOTES, *tokens)))
		expansions_quoted_treatment(tokens, splitok, flag);
	else
	{
		if (!(splitok = ft_expsplit(*tokens)))
			return (ERROR);
		ft_strdel(tokens);
		*tokens = expansions_management(splitok, 0, flag);
	}
	return (SUCCESS);
}
