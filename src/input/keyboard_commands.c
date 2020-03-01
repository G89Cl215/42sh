/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baavril <baavril@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 12:41:28 by baavril           #+#    #+#             */
/*   Updated: 2020/03/01 17:29:22 by baavril          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "keys.h"
#include "libft.h"
#include "input.h"
#include "error.h"
#include "prompt.h"
#include "display.h"
#include "history.h"

#include <unistd.h>
#include <stdio.h>
#include <term.h>
#include <curses.h>
#include <stdint.h>

extern int	g_retval;

int			keyboard_normal_char(union u_tc *term, char **buff, t_cursor *cursor)
{
	if (ft_isprint(term->key))
	{
		if (cursor->ctrl_r)
		{
			cursor->start = ft_strlen(*buff);
			normal_char(buff, cursor, term->key);
			cursor->match = get_history(buff, cursor);
			return (0);
		}
		normal_char(buff, cursor, term->key);
		return (0);
	}
	return (1);
}

int			keyboard_tabulation(union u_tc *term, char **buff, t_cursor *cursor)
{
	if (term->key == TABULATION)
	{
		if (cursor->ctrl_r)
		{
			tab_key(buff, cursor);
			ft_strdel(&(cursor->prompt));
			cursor->prompt_len = mkprompt(&(cursor->prompt));
			history(RESET, buff, NULL);
			return (0);
		}
		if (!(tab_key(buff, cursor)))
			return (-1);
		return (0);
	}
	return (1);
}

int			keyboard_enter(union u_tc *term, char **buff, t_cursor *cursor)
{
	if (term->key == ENTER)
	{
		if (cursor->ctrl_r)
		{
			update_buff(buff, cursor);
			history(RESET, buff, NULL);
			return (0);
		}
		return (0);
	}
	return (1);
}

int			keyboard_backspace(union u_tc *term, char **buff, t_cursor *cursor)
{
	if (term->key == BACKSPACE)
	{
		if (cursor->ctrl_r)
		{
			cursor->start = ft_strlen(*buff);
			backspace_key(buff, cursor);
			if (**buff)
				cursor->match = get_history(buff, cursor);
			else
			{
				cursor->match = NULL;
				cursor->match_ret = 1;
			}
			return (0);
		}
		backspace_key(buff, cursor);
		return (0);
	}
	return (1);
}

int			keyboard_ctrl_l(union u_tc *term, char **buff, t_cursor *cursor)
{
	if (term->key == CTRL_L)
	{
		if (cursor->ctrl_r)
		{
			update_buff(buff, cursor);
			ft_strdel(&(cursor->prompt));
			cursor->prompt_len = mkprompt(&(cursor->prompt));
			cursor->start = ft_strlen(*buff);
			ft_putstr(tgetstr("ho", NULL));
			return (0);
		}
		ft_putstr(tgetstr("ho", NULL));
	}
	return (1);
}

static int	keyboard_ctrl_c_wiper(char **buff, t_cursor *cursor)
{
	char	*tmp;
	int		tmp_len;

	tmp = NULL;
	tmp_len = 0;
	ft_strdel(buff);
	*buff = ft_strdup("");
	tmp = cursor->prompt;
	tmp_len = cursor->prompt_len;
	ft_init_cursor(cursor);
	cursor->prompt = tmp;
	cursor->prompt_len = tmp_len;
	ft_putchar('\n');
	g_retval = 130;
	return (0);
}

int			keyboard_ctrl_c(union u_tc *term, char **buff, t_cursor *cursor)
{
	if (term->key == CTRL_C)
	{
		if (cursor->ctrl_r)
		{
			update_buff(buff, cursor);
			cursor->prompt_len = mkprompt(&(cursor->prompt));
			cursor->start = ft_strlen(*buff);
			keyboard_ctrl_c_wiper(buff, cursor);
			return (0);
		}
		keyboard_ctrl_c_wiper(buff, cursor);
	}
	return (1);
}

int			keyboard_ctrl_d(union u_tc *term, char **buff, t_cursor *cursor)
{
	if (term->key == CTRL_D)
	{
		if (cursor->ctrl_r && cursor->match)
		{
			update_buff(buff, cursor);
			delete_key(buff, cursor);
			cursor->prompt_len = mkprompt(&(cursor->prompt));
			return (0);
		}
		if (*buff && **buff)
			delete_key(buff, cursor);
		else
		{
			ft_putendl("exit");
			if (ft_clean_exit(NULL, g_retval))
				g_retval = 146;
		}
	}
	return (1);
 }
