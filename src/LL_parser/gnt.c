/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bprunevi <bprunevi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/15 12:58:25 by bprunevi          #+#    #+#             */
/*   Updated: 2020/01/27 18:49:29 by bprunevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "tokens.h"
#include "libft.h"
#include "expansions.h"
#include "globing.h"

static void		fill_stack(t_list **stack, char *input)
{
	char **tmp1;
	char **tmp2;
	t_token tok;

	tmp1 = NULL;
	tok = get_next_token(input);
	if (tok.type == WORD && (!(ft_isin(OP_BRACE, tok.symbol)
	&& ft_isin(CL_BRACE, tok.symbol)))
	&& ((ft_isin('/', tok.symbol) && ft_isin(STAR, tok.symbol))
	|| ft_isin(STAR, tok.symbol))) 
	{
		tmp1 = ft_strsplit(tok.symbol, "/");
		tmp2 = globing(tmp1, tok.symbol);
		ft_tabdel(&tmp1);
		tmp1 = tmp2;
		ft_strdel(&(tok.symbol));
		while (*tmp1)
		{
			tok.symbol = *tmp1;
			tok.type = WORD;
			ft_lstadd(stack, ft_lstnew(&tok, sizeof(t_token)));
			tmp1++;
		}
		free(tmp2);
	}
	else
		ft_lstadd(stack, ft_lstnew(&tok, sizeof(t_token)));
}

t_token	gnt(char *input, int future)
{
	static t_list *stack = NULL;
	t_list *tmp_stack = NULL;
	t_token tmp_tok;

	if (!stack)
		fill_stack(&stack, input);
	tmp_tok = *((t_token *)stack->content);
	if (!future)
	{
		tmp_stack = stack->next;
		free(stack->content);
		free(stack);
		stack = tmp_stack;
	}
	return(tmp_tok);
}