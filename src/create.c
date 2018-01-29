/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 19:56:20 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 20:25:27 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

extern t_tar_options	g_tar_options;

t_tar_header	*create_header(char *filename)
{
	struct stat		st;
	t_tar_header	*header;

	if (lstat(filename, &st))
	{
		fprintf(stderr,
			"ft_tar: %s: Cannot stat: %s\n", filename, strerror(errno));
		return (NULL);
	}
	header = calloc(1, sizeof(t_tar_header));
	fill_header_type(header, st.st_mode, filename);
	fill_header_name(header, filename);
	sprintf(header->file_mode, "%06o ", st.st_mode & 0777);
	sprintf(header->owner_id, "%06o ", st.st_uid);
	sprintf(header->group_id, "%06o ", st.st_gid);
	fill_header_file_size(header, st.st_size);
	fill_header_mod_time(header, st.st_mtime);
	fill_header_link_name(header, filename);
	memcpy(header->ustar, TMAGIC, TMAGLEN);
	memcpy(header->ustar_ver, TVERSION, TVERSLEN);
	fill_header_owner_info(header, st.st_uid, st.st_gid);
	fill_header_device_info(header, st.st_rdev);
	calculate_checksum(header);
	return (header);
}

void			check_input_files(void)
{
	if (!g_tar_options.input_files_len)
	{
		fprintf(stderr, "ft_tar: no files or directories specified\n");
		exit(1);
	}
}

void			tar_create(void)
{
	unsigned char	end_block[1024];
	FILE			*archive;
	int				i;

	check_input_files();
	if (g_tar_options.archive_name)
		archive = fopen(g_tar_options.archive_name, "wb");
	else
		archive = stdout;
	i = 0;
	while (i < g_tar_options.input_files_len)
		add_file_to_archive(archive, g_tar_options.input_files[i++]);
	memset(end_block, 0, 1024);
	fwrite(end_block, 1, sizeof(end_block), archive);
	fclose(archive);
}
