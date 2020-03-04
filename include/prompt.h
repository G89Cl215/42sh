/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bprunevi <bprunevi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 18:33:04 by bprunevi          #+#    #+#             */
/*   Updated: 2020/02/29 15:59:18 by bprunevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPT_H
# define PROMPT_H

# include "libft.h"
# include "input.h"

size_t	search_prompt(t_cursor *cursor, char *buff);
size_t	mkprompt(char **buff);
size_t	mkprompt_quote(char *input, char **buff);

#endif
