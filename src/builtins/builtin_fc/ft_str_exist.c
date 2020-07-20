/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_exist.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgouedar <tgouedar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/20 13:19:00 by tgouedar          #+#    #+#             */
/*   Updated: 2020/07/20 13:44:12 by tgouedar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_fc.h"

int			str_exist(char **argv)
{
	char	*cmd;

	history(FIRST, NULL, &cmd);
	if (ft_strisnbr(argv[0]))
	{
		if (ft_strfchr(argv[1], cmd))
			return (0);
		while (history(FORWARD, NULL, &cmd) != 2)
			if (ft_strfchr(argv[1], cmd))
				return (0);
	}
	else
	{
		if (ft_strfchr(argv[0], cmd))
			return (0);
		while (history(FORWARD, NULL, &cmd) != 2)
			if (ft_strfchr(argv[0], cmd))
				return (0);
	}
	return (1);
}
