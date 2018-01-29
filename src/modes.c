/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 19:53:37 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 20:25:47 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

extern t_tar_options	g_tar_options;

char			check_enabled_modes(char c)
{
	char error_flag;

	if (g_tar_options.mode == EXTRACT && c != 'x')
		error_flag = 'x';
	else if (g_tar_options.mode == CREATE && c != 'c')
		error_flag = 'c';
	else if (g_tar_options.mode == LIST && c != 't')
		error_flag = 't';
	else
		error_flag = 0;
	return (error_flag);
}

void			enable_mode(char c)
{
	char error_flag;

	if ((error_flag = check_enabled_modes(c)))
	{
		fprintf(stderr,
			"ft_tar: can't specify both -%c and -%c\n", error_flag, c);
		exit(1);
	}
	if (c == 'x')
		g_tar_options.mode = EXTRACT;
	else if (c == 'c')
		g_tar_options.mode = CREATE;
	else if (c == 't')
		g_tar_options.mode = LIST;
}
