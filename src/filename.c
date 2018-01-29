/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 17:35:08 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 17:47:16 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

char	*make_file_name(t_tar *t)
{
	char	*data;
	int		len;

	data = ft_memalloc(260);
	data = strncat(data, t->file_prefix, 155);
	len = strlen(data);
	if ((len != 0) && (data[len - 1] != '/'))
		strcat(data, "/");
	data = strncat(data, t->file_name, 100);
	return (data);
}

void	print_filename(char *filename, int verbose)
{
	if ((errno != 0) && (errno != EEXIST))
		printf("x %s: %s\n", filename, strerror(errno));
	if (verbose == 1)
		printf("x %s\n", filename);
	return ;
}
