/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modtime.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 17:33:32 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 17:48:34 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

void	set_modtime(char *filename, t_tar *t)
{
	struct timeval times[2];

	times[0].tv_sec = strtoul(t->last_mod, NULL, 8);
	times[0].tv_usec = 0;
	times[1].tv_sec = strtoul(t->last_mod, NULL, 8);
	times[1].tv_usec = 0;
	utimes(filename, times);
	return ;
}

void	restore_modtime(char *data, size_t size, int restore)
{
	size_t	i;
	size_t	filesize;
	size_t	pad;
	char	*filename;
	t_tar	*t;

	i = 0;
	while ((restore == 1) && (i < size))
	{
		t = (t_tar *)&data[i];
		if (are_next_two_blocks_empty(data, i, size) == 1)
			break ;
		filename = make_file_name(t);
		filesize = strtol(t->file_size, NULL, 8);
		set_modtime(filename, t);
		free(filename);
		i += 512;
		pad = ((((filesize / 512) + 1) * 512) - filesize);
		i += filesize;
		if (pad != 512)
			i += pad;
	}
	return ;
}
