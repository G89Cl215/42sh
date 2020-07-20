/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baavril <baavril@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/06 20:52:32 by baavril           #+#    #+#             */
/*   Updated: 2020/07/20 16:49:39 by tgouedar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "shell_variables.h"
#include "expansions.h"
#include "libft.h"

static int		reachquotelim(char *str, char c)
{
	int		i;

	i = 1;
	while (str[i] && str[i] != c)
	{
		++i;
		if (c != SQUOTES && str[i] == c && str[i - 1] == BSLASH)
			++i;
	}
	return (i);
}

int				getquotelim(char *str)
{
	int		i;
	int		bck_slash;

	i = 0;
	if (str[i] == DQUOTES)
		return (reachquotelim(&str[i], DQUOTES));
	else if (str[i] == SQUOTES)
		return (reachquotelim(&str[i], SQUOTES));
	else if (str[i] != SQUOTES && str[i] != DQUOTES)
	{
		bck_slash = (str[i] == '\\');
		while ((str[i + 1] && str[i + 1] != DQUOTES && str[i + 1] != SQUOTES)
		|| (bck_slash))
		{
			++i;
			bck_slash = !bck_slash && (str[i] == '\\');
		}
		if (!(str[i + 1]))
			++i;
	}
	return (i);
}
