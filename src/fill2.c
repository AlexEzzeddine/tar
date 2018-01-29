/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 20:00:22 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 20:00:37 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

void			fill_header_file_size(t_tar_header *header, off_t st_size)
{
	if (*(header->file_type) == DIRTYPE)
		st_size = 0;
	sprintf(header->file_size, "%011o", (int)st_size);
	header->file_size[11] = ' ';
}

void			fill_header_mod_time(t_tar_header *header, time_t mtime)
{
	sprintf(header->last_mod, "%011o", (int)mtime);
	header->last_mod[11] = ' ';
}
