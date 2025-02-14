/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgouedar <tgouedar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/15 17:59:53 by tgouedar          #+#    #+#             */
/*   Updated: 2020/07/13 13:37:56 by tgouedar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stdlib.h>
# include "tokens.h"

int					ft_trim_comment(void);
int					ft_istoken(const char *str);
int					ft_is_quote(char c);
struct s_token		get_next_token(char *str);
t_token				get_next_token_alias(char *str);
void				ft_free_token(t_token *tok);
char				*cpy_without_bn(char *dst, const char *src, int n);
char				*get_token_symbol(int token);
t_token				ft_delimit_token(char **index);
t_token				tokenization(char type, char *value);
void				ft_quote(const char *s, int *i);
int					ft_quote_check(const char *s, int *i, char *quote_t,
															_Bool *open_q);

#endif
