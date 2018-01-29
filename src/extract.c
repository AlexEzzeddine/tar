/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 17:31:12 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 21:29:07 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

void	recreate_directory(t_tar *t, size_t *i, int verbose)
{
	char	*filename;

	filename = make_file_name(t);
	errno = 0;
	mkdir(filename, strtol(t->file_mode, NULL, 8));
	print_filename(filename, verbose);
	chmod(filename, strtol(t->file_mode, NULL, 8));
	free(filename);
	*i += 512;
	return ;
}

void	recreate_file(t_tar *t, size_t *i, int verbose)
{
	char	*fn;
	size_t	fs;
	size_t	pad;

	fn = make_file_name(t);
	fs = strtol(t->file_size, NULL, 8);
	errno = 0;
	put_file_contents(fn, &((unsigned char *)t)[512], fs);
	print_filename(fn, verbose);
	chmod(fn, strtol(t->file_mode, NULL, 8));
	free(fn);
	*i += 512;
	pad = ((((fs / 512) + 1) * 512) - fs);
	*i += fs;
	if (pad != 512)
		*i += pad;
	return ;
}

void	extract_file(t_tar *t, size_t *i, int verbose)
{
	if (t->file_type[0] == DIRTYPE)
		recreate_directory(t, i, verbose);
	else if ((t->file_type[0] == REGTYPE) || (t->file_type[0] == AREGTYPE))
		recreate_file(t, i, verbose);
	return ;
}

int		extract(char *file, int restore, int verbose)
{
	t_tar	*t;
	size_t	i;
	size_t	size;
	char	*data;

	if ((data = getfilecontents(file, &size)) == NULL)
	{
		printf("error: could not read file\n");
		return (0);
	}
	i = 0;
	while (i < size)
	{
		t = (t_tar *)&data[i];
		if (is_valid_header(t))
			extract_file(t, &i, verbose);
		else
			break ;
		if (are_next_two_blocks_empty(data, i, size) == 1)
			break ;
	}
	restore_modtime(data, size, restore);
	free(data);
	return (1);
}
