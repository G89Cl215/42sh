/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_interpreter.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bprunevi <bprunevi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/19 11:44:12 by bprunevi          #+#    #+#             */
/*   Updated: 2019/12/14 13:08:54 by bprunevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "parser.h"
#include "lexer.h"
#include "libft.h"

/*
 * comp_list	: and_or SEMI comp_list
 */
t_node	*comp_list(t_token tok)
{
	t_node	*node;
	t_node	*tmp1;
	t_node	*tmp2;

	if ((tmp1 = and_or(tok)))
	{
		if (is_potential(peek(), N_SEMI))
		{
			gnt(NULL);
			if ((tmp2 = comp_list(gnt(NULL))))
			{
				node = malloc(sizeof(t_node));
				node->left.v = tmp1;
				node->right.v = tmp2;
				node->f = i_comp_list;
				ft_printf("comp_list full with 2 elems was created\n");
				return(node);
			}
		}
		return(tmp1);
	}
	return(NULL);
}

void	interpret(t_node *node)
{
	if (node)
	{
		ft_printf("interpreting a command..\n");
		node->f(node->left, node->right);
		ft_printf("FT_LAUNCH_JOB(FOREGROUND)\n");
	}
	if (!node)
		ft_printf("interpreting empty string..\n");
	ft_printf("_______________________________________________\n");
}

void	debug_parser(char *input)
{
	interpret(comp_list(get_next_token(input)));
	gnt(NULL);
}
