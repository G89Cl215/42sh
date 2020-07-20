/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_bslash.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baavril <baavril@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/06 20:52:32 by baavril           #+#    #+#             */
/*   Updated: 2020/07/20 16:53:39 by tgouedar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "shell_variables.h"
#include "expansions.h"
#include "libft.h"

static t_quoted			*ft_delimit_quoted_token(char *str, int *j)
{
	int					lim;
	int					plus;
	t_quoted			*q_lst;

	plus = 0;
	if (!(q_lst = (struct s_quoted*)malloc(sizeof(struct s_quoted))))
		return (NULL);
	q_lst->expand = (str[*j] == '\'');
	q_lst->next = NULL;
	lim = getquotelim(&str[*j]);
	if ((str[*j]) && str[*j] != SQUOTES && str[*j] != DQUOTES
	&& (str[lim + *j]))
		plus = 1;
	if (str[*j] && (str[*j] == DQUOTES || str[*j] == SQUOTES))
	{
		++(*j);
		lim = (lim) ? lim - 1 : 0;
	}
	q_lst->token = dupbtwqlim(&str[*j], lim + plus);
	*j += lim + 1;
	return (q_lst);
}

t_quoted				*token_quotes_generator(char *str)
{
	t_quoted		*quoted_tokens;
	t_quoted		*voyager;
	int				len;
	int				j;

	j = 0;
	quoted_tokens = NULL;
	len = ft_strlen(str);
	while (j < len)
	{
		if (!quoted_tokens)
		{
			quoted_tokens = ft_delimit_quoted_token(str, &j);
			voyager = quoted_tokens;
		}
		else
		{
			voyager->next = ft_delimit_quoted_token(str, &j);
			voyager = voyager->next;
		}
	}
	return (quoted_tokens);
}
