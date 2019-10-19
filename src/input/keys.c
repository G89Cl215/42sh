/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bprunevi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 18:02:02 by bprunevi          #+#    #+#             */
/*   Updated: 2019/10/19 15:01:49 by bprunevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "input.h"
#include "history.h"

#include <unistd.h>
#include <term.h>
#include <curses.h>

char	*buff_realloc(char *old_buff, int i)
{
	static int buff_size = 0;
	char *new_buff;

	if (!*old_buff)
		buff_size = 0;
	if (i > INPUT_SIZE * buff_size)
	{
		buff_size = i / INPUT_SIZE + 1;
		if (!(new_buff = malloc(1 + (buff_size) * INPUT_SIZE)))
			exit(-1);
		ft_strcpy(new_buff, old_buff);
		free(old_buff);
		return (new_buff);
	}
	return (old_buff);
}

void normal_char(char **buff, int *j, int *i, char c)
{
	*buff = buff_realloc(*buff, ++(*i));
	ft_memmove(&((*buff)[*j + 1]), &((*buff)[*j]), *i - *j);
	(*buff)[(*j)++] = c;
}

void set_string(char **buff, int *j, int *i, char *str)
{
	int len;

	if (!str)
		return;
	len = ft_strlen(str);
	if (*i < len)
		*buff = buff_realloc(*buff, len);
	ft_strcpy(*buff, str);
	*i = len;
	*j = len;
}

void right_arrow(char **buff, int *j, int *i)
{
	(void) buff;
	if (*j < *i)
		++(*j);
}

void left_arrow(char **buff, int *j, int *i)
{
	(void) buff;
	(void) i;
	if (*j > 0)
		--(*j);
}

void delete_key(char **buff, int *j, int *i)
{
	if (*j < *i && *i > 0)
	{
		ft_memmove(&((*buff)[*j]), &((*buff)[*j + 1]), *i - *j);
		--(*i);
	}
}

void backspace_key(char **buff, int *j, int *i)
{
	if (*i > 0 && *j > 0)
	{
		--(*j);
		ft_memmove(&((*buff)[*j]), &((*buff)[*j + 1]), *i - *j);
		--(*i);
	}
}

void tab_key(char **buff, int *j, int *i)
{
	char *str = ft_strdup("tabulation");
	while (*str)
		normal_char(buff, j, i, *str++);
}

void down_arrow(char **buff, int *j, int *i)
{
	char *str;
	if (!inside_history)
		return;
	else if (history(FORWARD, NULL, &str) == 2)
	{
		set_string(buff, i, j, inside_history);
		ft_strdel(&inside_history);
	}
	else
		set_string(buff, i, j, str);
}

void up_arrow(char **buff, int *j, int *i)
{
	char *str;
	if (!inside_history && (inside_history = ft_strdup(*buff)))
		history(LAST, NULL, &str);
	else
		history(BACKWARD, NULL, &str);
	set_string(buff, i, j, str);
}

void paste_key(char **buff, int *j, int *i)
{
	char *str = copybuff;
	if (str)
		while (*str)
			normal_char(buff, j, i, *str++);
}

void home_key(char **buff, int *j, int *i)
{
	(void) buff;
	(void) i;
	*j = 0;
}

void end_key(char **buff, int *j, int *i)
{
	(void) buff;
	*j = *i;
}

void next_word(char **buff, int *j, int *i)
{
	(void) i;
	while(*j < *i && ft_isalnum((*buff)[*j]))
			(*j)++;
	while(*j < *i && !ft_isalnum((*buff)[*j]))
			(*j)++;
}

void previous_word(char **buff, int *j, int *i)
{
	(void) i;
	while(*j > 0 && ft_isalnum((*buff)[*j]))
			(*j)--;
	while(*j > 0 && !ft_isalnum((*buff)[*j]))
			(*j)--;
}

void select_key(char **buff, int *j, int *i, int *u)
{
	(void) i;
	if (*u == -1)
		*u = *j;
	else if (*u != *j)
	{
		ft_strdel(&copybuff);
		if (*j > *u)
			copybuff = ft_strndup(*buff + *u, *j - *u);
		else if (*j < *u)
			copybuff = ft_strndup(*buff + *j, *u - *j);
		*u = -1;
	}
}
void escape_char(char **buff, int *j, int *i, int *u)
{
	char input_buffer[16];

	ft_bzero(input_buffer, 8);
	read(0, input_buffer, 8);

	if (!ft_strcmp(&input_buffer[1], tgetstr("kl", NULL) + 2))
		left_arrow(buff, j, i);
	else if (!ft_strcmp(&input_buffer[1], tgetstr("kr", NULL) + 2))
		right_arrow(buff, j, i);
	if (!ft_strcmp(&input_buffer[1], tgetstr("ku", NULL) + 2))
		up_arrow(buff, j, i);
	if (!ft_strcmp(&input_buffer[1], tgetstr("kd", NULL) + 2))
		down_arrow(buff, j, i);
	else if (!ft_strcmp(&input_buffer[1], tgetstr("kD", NULL) + 2))
		delete_key(buff, j, i);
	else if (!ft_strcmp(&input_buffer[1], tgetstr("kh", NULL) + 2))
		home_key(buff, j, i);
	else if (!ft_strcmp(&input_buffer[1], tgetstr("kN", NULL) + 2))
		next_word(buff, j, i);
	else if (!ft_strcmp(&input_buffer[1], tgetstr("kP", NULL) + 2))
		previous_word(buff, j, i);
	else if (!ft_strcmp(&input_buffer[1], "F\0")) //FAUX MAIS TEMP
		end_key(buff, j, i);
	else if (!ft_strcmp(&input_buffer[1], "1;2A")) //FAUX MAIS TEMP
		select_key(buff, j, i, u);
	else if (!ft_strcmp(&input_buffer[1], "1;2B")) //FAUX MAIS TEMP
		paste_key(buff, j, i);
}
