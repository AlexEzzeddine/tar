/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 17:25:32 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 17:44:58 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

char	*time_string(t_tar *t)
{
	time_t	now;
	char	*result;
	int		i;
	time_t	filetime;

	filetime = strtol(t->last_mod, NULL, 8);
	now = time(&now);
	result = ctime(&filetime);
	if ((filetime + ((365 / 2) * 86400) < now) ||
			(now + ((365 / 2) * 86400) < filetime))
	{
		i = 0;
		while (i < 5)
		{
			result[11 + i] = result[19 + i];
			i++;
		}
	}
	return (&result[4]);
}

void	print_details(char *filename, t_tar *t, size_t filesize)
{
	unsigned long	st_mode;
	static int		olen;
	static int		glen;
	static int		size_length;

	st_mode = strtol(t->file_mode, NULL, 8);
	print_file_mode(t, st_mode);
	printf("  0 ");
	if ((int)strlen(t->owner_name) > olen)
		olen = strlen(t->owner_name);
	if ((int)strlen(t->group_name) > glen)
		glen = strlen(t->group_name);
	printf("%-*s %-*s ", olen, t->owner_name, glen, t->group_name);
	if (count_digits(filesize) > size_length)
		size_length = count_digits(filesize);
	if (size_length < 5)
		size_length = 5;
	printf("%*zu ", size_length, filesize);
	printf("%.12s %s\n", time_string(t), filename);
}

void	print_long(char *data, size_t size)
{
	size_t	i;
	size_t	filesize;
	size_t	pad;
	char	*filename;
	t_tar	*t;

	i = 0;
	while (i < size)
	{
		t = (t_tar *)&data[i];
		if (are_next_two_blocks_empty(data, i, size) == 1)
			break ;
		filename = make_file_name(t);
		filesize = strtol(t->file_size, NULL, 8);
		print_details(filename, t, filesize);
		free(filename);
		i += 512;
		pad = ((((filesize / 512) + 1) * 512) - filesize);
		i += filesize;
		if (pad != 512)
			i += pad;
	}
	return ;
}

void	print_short(char *data, size_t size)
{
	size_t	i;
	size_t	filesize;
	size_t	pad;
	char	*filename;
	t_tar	*t;

	i = 0;
	while (i < size)
	{
		t = (t_tar *)&data[i];
		if (are_next_two_blocks_empty(data, i, size) == 1)
			break ;
		filename = make_file_name(t);
		printf("%s\n", filename);
		free(filename);
		i += 512;
		filesize = strtol(t->file_size, NULL, 8);
		pad = ((((filesize / 512) + 1) * 512) - filesize);
		i += filesize;
		if (pad != 512)
			i += pad;
	}
	return ;
}

void	print(char *file, int verbose)
{
	size_t	size;
	char	*data;
	t_tar	*t;

	if ((data = getfilecontents(file, &size)) == NULL)
	{
		printf("%s: error: could not read file\n", file);
		return ;
	}
	t = (t_tar *)&data[0];
	if (is_valid_header(t))
	{
		if (verbose)
			print_long(data, size);
		else
			print_short(data, size);
	}
	else
		printf("%s: error: invalid tar archive\n", file);
	free(data);
	return ;
}
