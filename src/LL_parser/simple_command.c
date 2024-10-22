/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bprunevi <bprunevi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/19 11:36:39 by bprunevi          #+#    #+#             */
/*   Updated: 2020/03/03 19:26:58 by yberramd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "parser.h"
#include "libft.h"
#include "builtins.h"

t_node		*cmd_suffix(t_token tok)
{
	t_node	*node;
	t_elem	tmp1;
	int		(*f)(t_elem left, t_elem right);

	if (((tmp1.v = io_redirect(tok))
	&& (f = i_redirect))
	|| ((tmp1.c = word(tok))
	&& (f = i_suffix_word)))
	{
		node = malloc(sizeof(t_node));
		node->left = tmp1;
		node->right.v = NULL;
		if (is_potenti(gnt(NULL, 1), N_SUFFIX))
			node->right.v = cmd_suffix(gnt(NULL, 0));
		node->f = f;
		return (node);
	}
	return (NULL);
}

t_node		*exec(t_token tok)
{
	t_node	*node;
	char	*tmp1;

	if ((tmp1 = cmd_word(tok)))
	{
		if (!(node = malloc(sizeof(t_node))))
			return (NULL);
		node->left.c = tmp1;
		node->right.v = NULL;
		if (is_potenti(gnt(NULL, 1), N_SUFFIX))
			node->right.v = cmd_suffix(gnt(NULL, 0));
		node->f = i_exec;
		return (node);
	}
	return (NULL);
}

t_node		*cmd_prefix(t_token tok)
{
	t_node	*node;
	t_elem	tmp1;
	int		(*f)(t_elem left, t_elem right);

	if (((tmp1.v = io_redirect(tok))
	&& (f = i_redirect))
	|| ((tmp1.c = assig_word(tok))
	&& (f = i_prefix)))
	{
		node = malloc(sizeof(t_node));
		node->left = tmp1;
		node->right.v = NULL;
		if (is_potenti(gnt(NULL, 1), N_PREFIX))
			node->right.v = cmd_prefix(gnt(NULL, 0));
		node->f = f;
		return (node);
	}
	return (NULL);
}

t_node		*simple_command(t_token tok)
{
	t_node *node;
	t_node *tmp1;

	if ((tmp1 = cmd_prefix(tok)))
	{
		if (is_potenti(gnt(NULL, 1), N_CMD_WORD))
		{
			node = malloc(sizeof(t_node));
			node->left.v = tmp1;
			node->right.v = exec(gnt(NULL, 0));
			node->f = i_simple_command;
			return (node);
		}
		return (tmp1);
	}
	else if (is_potenti(tok, N_CMD_NAME))
	{
		node = malloc(sizeof(t_node));
		node->left.v = NULL;
		node->right.v = exec(tok);
		node->f = i_simple_command;
		return (node);
	}
	return (NULL);
}

t_node		*command(t_token tok)
{
	return (simple_command(tok));
}
