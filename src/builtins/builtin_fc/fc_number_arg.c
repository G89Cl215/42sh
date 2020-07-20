/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fc_number_arg.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgouedar <tgouedar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/13 17:30:57 by tgouedar          #+#    #+#             */
/*   Updated: 2020/07/20 13:20:57 by tgouedar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_fc.h"

/*
**  Any horrible naming in this file comes from a 80+ line function I had to
** split into norminette standards...
**  I didn't code this function, I do not know what it does an I am NOT willing
** to spend half a day finding out. so I did quick qnd messy.
*/

static void			ft_next_history(int flag, t_fc *explorer)
{
	history(flag, NULL, &(explorer->cmd));
	if (flag == FORWARD)
		(explorer->i)--;
	else if (flag == BACKWARD)
		(explorer->i)++;
}

static void			ft_plop_un(char **argv, t_fc *explorer, int max, int nbr)
{
	if (explorer->i == 0)
	{
		history(LAST, NULL, &(explorer->cmd));
		while ((explorer->found = ft_strfchr(argv[0], explorer->cmd)) == 0
		&& (max - explorer->i) > nbr)
			ft_next_history(BACKWARD, explorer);
		while (explorer->found == 0 && ft_strfchr(argv[0], explorer->cmd) == 0)
			ft_next_history(BACKWARD, explorer);
	}
	else if (explorer->found > 0 && (max - explorer->i) > nbr)
		ft_next_history(BACKWARD, explorer);
	else if (explorer->found == 0 && (max - explorer->i) < nbr)
	{
		history(FORWARD, NULL, &(explorer->cmd));
		(explorer->i)++;
	}
}

static void			ft_plop_zero(char **argv, t_fc *explorer, int max, int nbr)
{
	if (explorer->i == 0)
	{
		history(LAST, NULL, &(explorer->cmd));
		while ((explorer->found = ft_strfchr(argv[1], explorer->cmd)) == 0
		&& (max - explorer->i) > nbr)
			ft_next_history(BACKWARD, explorer);
		if (explorer->found == 1)
		{
			explorer->found = max - explorer->i;
			while ((max - explorer->i) > nbr
			&& history(BACKWARD, NULL, &(explorer->cmd)))
				(explorer->i)++;
		}
	}
	else if (explorer->found > 0 && (max - explorer->i) < explorer->found)
		ft_next_history(FORWARD, explorer);
	else if (explorer->found == 0)
	{
		if (ft_strfchr(argv[1], explorer->cmd) != 0)
			return ;
		ft_next_history(BACKWARD, explorer);
	}
}

static int			ft_set_arg_nbr(char **argv, int max)
{
	int		nbr;

	if (ft_strisnbr(argv[0]))
		nbr = ft_atoi_history(argv[0]);
	else
		nbr = ft_atoi_history(argv[1]);
	if (nbr == 0)
		nbr = INT_MAX;
	if (nbr < 0)
		if ((nbr = max + nbr) < 0)
			nbr = 1;
	return (nbr);
}

int					f_arg_nbr(char **argv, int max, int reset)
{
	int				nbr;
	static t_fc		explorer = {0, 0, NULL};

	if (reset)
		return (reset_arg(&explorer));
	if (explorer.i == 0 && str_exist(argv))
		return (-2);
	if ((!ft_strisnbr(argv[0]) && !ft_strisnbr(argv[0]))
	|| (nbr = ft_set_arg_nbr(argv, max)) <= 0)
		return (-1);
	if (ft_strisnbr(argv[0]))
		ft_plop_zero(argv, &explorer, max, nbr);
	else
		ft_plop_un(argv, &explorer, max, nbr);
	return (max - explorer.i);
}
