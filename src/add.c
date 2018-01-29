/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 19:58:29 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 20:04:00 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

void	add_header_to_archive(t_tar_header *header, FILE *file)
{
	static unsigned char	padding[12] = {
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	};

	fwrite(header, 1, sizeof(t_tar_header), file);
	fwrite(padding, 1, sizeof(padding), file);
}

void	add_content_to_archive(FILE *archive, FILE *file)
{
	char block[512];

	while (!feof(file))
	{
		memset(block, 0, sizeof(block));
		if (fread(block, 1, sizeof(block), file))
			fwrite(block, 1, sizeof(block), archive);
	}
}

void	add_children_to_archive(FILE *archive, char *directory)
{
	DIR				*dir_stream;
	struct dirent	*dir_entity;

	dir_stream = opendir(directory);
	if (dir_stream)
	{
		while ((dir_entity = readdir(dir_stream)))
			if (strcmp(dir_entity->d_name, ".")
			&& strcmp(dir_entity->d_name, ".."))
				add_file_to_archive(archive,
				join_path(directory, dir_entity->d_name));
		closedir(dir_stream);
	}
	else
	{
		fprintf(stderr,
			"ft_tar: Could not open file %s: %s\n", directory, strerror(errno));
		return ;
	}
}

void	add_file_to_archive(FILE *archive, char *filename)
{
	t_tar_header	*header;
	FILE			*file;
	int				dir;

	if (!(header = create_header(filename)))
		return ;
	if (!(file = fopen(filename, "rb")))
	{
		fprintf(stderr,
			"ft_tar: Could not open file %s: %s\n", filename, strerror(errno));
		return ;
	}
	log_file(filename);
	add_header_to_archive(header, archive);
	dir = (*(header->file_type) == DIRTYPE) ? 1 : 0;
	free(header);
	if (!dir)
		add_content_to_archive(archive, file);
	else
		add_children_to_archive(archive, filename);
}
