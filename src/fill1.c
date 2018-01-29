/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 19:54:23 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 19:56:14 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

void			fill_header_type(t_tar_header *header,
	mode_t mode, char *filename)
{
	if (S_ISREG(mode))
		*(header->file_type) = REGTYPE;
	else if (S_ISDIR(mode))
		*(header->file_type) = DIRTYPE;
	else if (S_ISFIFO(mode))
		*(header->file_type) = FIFOTYPE;
	else if (S_ISCHR(mode))
		*(header->file_type) = CHRTYPE;
	else if (S_ISBLK(mode))
		*(header->file_type) = BLKTYPE;
	else if (S_ISLNK(mode))
		*(header->file_type) = SYMTYPE;
	else if (S_ISSOCK(mode))
		fprintf(stderr, "Error: Cannot tar socket file %s\n", filename);
	else
		fprintf(stderr, "Error: Unknown file type for file %s\n", filename);
}

void			fill_header_name(t_tar_header *header, char *filename)
{
	int len;
	int offset;

	if (*(header->file_type) == DIRTYPE)
		filename = join_path(filename, "");
	len = strlen(filename);
	offset = 0;
	if (len > 100)
		offset = len - 100;
	memcpy(header->file_name, filename + offset, len - offset);
	memcpy(header->file_prefix, filename, offset);
	if (*(header->file_type) == DIRTYPE)
		free(filename);
}

void			fill_header_link_name(t_tar_header *header, char *filename)
{
	if (*(header->file_type) != SYMTYPE)
		return ;
	if (readlink(filename, header->name_of_link, 100) < 0)
	{
		fprintf(stderr,
			"Error: Could not read link %s: %s\n", filename, strerror(errno));
		exit(1);
	}
}

void			fill_header_owner_info(t_tar_header *header,
	uid_t uid, gid_t gid)
{
	struct passwd	*pwd;
	struct group	*grp;

	if ((pwd = getpwuid(uid)))
		strncpy(header->owner_name, pwd->pw_name, 32);
	if ((grp = getgrgid(gid)))
		strncpy(header->group_name, grp->gr_name, 32);
}

void			fill_header_device_info(t_tar_header *header, dev_t st_rdev)
{
	sprintf(header->dev_major, "%06o ", major(st_rdev));
	sprintf(header->dev_minor, "%06o ", minor(st_rdev));
}
