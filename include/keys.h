/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bprunevi <bprunevi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/26 13:40:01 by bprunevi          #+#    #+#             */
/*   Updated: 2019/09/26 13:41:22 by bprunevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYS_H
#define KEYS_H

void right_arrow(char **buff, int *j, int *i);
void left_arrow(char **buff, int *j, int *i);
void delete_key(char **buff, int *j, int *i);
void backspace_key(char **buff, int *j, int *i);
void normal_char(char **buff, int *j, int *i, char c);
void escape_char(char **buff, int *j, int *i);

#endif
