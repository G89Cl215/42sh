/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_sequence.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bprunevi <bprunevi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/19 11:48:12 by bprunevi          #+#    #+#             */
/*   Updated: 2020/02/26 16:11:49 by bprunevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "parser.h"
#include "libft.h"
#include <unistd.h>

extern int g_parsingerr;

static t_node	*pipe_sequence(t_token tok)
{
	t_node		*node;
	t_node		*tmp1;
	t_node		*tmp2;
	extern int	g_alias_treated;

	if ((tmp1 = command(tok)))
	{
		if (is_potential(peek(), N_PIPE))
		{
			g_alias_treated = 0;
			eat();
			if (!(tmp2 = pipe_sequence(eat()))
				&& (g_parsingerr = 1))
				ft_dprintf(STDERR_FILENO, "parsing error near pipe\n");
			node = malloc(sizeof(t_node));
			node->left.v = tmp1;
			node->right.v = tmp2;
			node->f = i_pipe_sequence;
			return (node);
		}
		return (tmp1);
	}
	return (NULL);
}

static t_node	*pipeline(t_token tok)
{
	return (pipe_sequence(tok));
}

t_node			*and_or(t_token tok)
{
	t_node	*node;
	t_node	*tmp1;
	t_node	*tmp2;
	int		(*f)(t_elem left, t_elem right);

	if ((tmp1 = pipeline(tok)))
	{
		if ((is_potential(peek(), N_AND_IF) && (f = i_and_op))
		|| (is_potential(peek(), N_OR_IF) && (f = i_or_op)))
		{
			eat();
			if (!(tmp2 = and_or(eat()))
				&& (g_parsingerr = 1))
				ft_dprintf(STDERR_FILENO, "parsing error near logical op\n");
			node = malloc(sizeof(t_node));
			node->left.v = tmp1;
			node->right.v = tmp2;
			node->f = f;
			return (node);
		}
		return (tmp1);
	}
	return (NULL);
}

t_node			*comp_list(t_token tok)
{
	t_node	*node;
	t_node	*tmp1;

	if ((tmp1 = and_or(tok)))
	{
		node = malloc(sizeof(t_node));
		node->left.v = tmp1;
		node->f = i_comp_list;
		if (is_potential(peek(), N_SEMI))
			eat();
		if (is_potential(peek(), N_AND))
		{
			eat();
			node->f = i_and_list;
		}
		node->right.v = comp_list(eat());
		return (node);
	}
	return (NULL);
}
