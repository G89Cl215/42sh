/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abarthel <abarthel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/16 13:12:45 by abarthel          #+#    #+#             */
/*   Updated: 2019/11/09 12:09:59 by tgouedar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOB_H
# define JOB_H

# include <sys/types.h>

int				job(char **argv, char **envp);
int				path_concat(char **bin);

extern pid_t	g_childpid;

#endif
