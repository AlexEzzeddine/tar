/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tar.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 20:01:09 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 20:26:12 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

extern t_tar_options	g_tar_options;

void			ft_tar(void)
{
	t_tar_mode mode;

	mode = g_tar_options.mode;
	if (mode == NONE)
	{
		fprintf(stderr, "ft_tar: must specify one of -c, -x, -t\n");
		exit(1);
	}
	else if (mode == CREATE)
		tar_create();
// else if (mode == EXTRACT)
//  tar_extract();
// else if (mode == LIST)
//  tar_list();
}
