/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   permissions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 17:27:21 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 17:45:28 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

void	print_entry_type(t_tar *t)
{
	if ((t->file_type[0] == REGTYPE) || (t->file_type[0] == AREGTYPE))
		printf("-");
	else if (t->file_type[0] == DIRTYPE)
		printf("d");
	else if (t->file_type[0] == LNKTYPE)
		printf("l");
	else if (t->file_type[0] == CHRTYPE)
		printf("c");
	else if (t->file_type[0] == BLKTYPE)
		printf("b");
	else if (t->file_type[0] == FIFOTYPE)
		printf("p");
	return ;
}

void	print_permissions(unsigned long st_mode)
{
	(st_mode & S_IRUSR) ? printf("r") : printf("-");
	(st_mode & S_IWUSR) ? printf("w") : printf("-");
	if (st_mode & S_ISUID)
		(st_mode & S_IXUSR) ? printf("s") : printf("S");
	else
		(st_mode & S_IXUSR) ? printf("x") : printf("-");
	(st_mode & S_IRGRP) ? printf("r") : printf("-");
	(st_mode & S_IWGRP) ? printf("w") : printf("-");
	if (st_mode & S_ISGID)
		(st_mode & S_IXGRP) ? printf("s") : printf("S");
	else
		(st_mode & S_IXGRP) ? printf("x") : printf("-");
	(st_mode & S_IROTH) ? printf("r") : printf("-");
	(st_mode & S_IWOTH) ? printf("w") : printf("-");
	if (st_mode & S_ISVTX)
		(st_mode & S_IXOTH) ? printf("t") : printf("T");
	else
		(st_mode & S_IXOTH) ? printf("x") : printf("-");
}

void	print_file_mode(t_tar *t, unsigned long st_mode)
{
	(void)print_entry_type(t);
	(void)print_permissions(st_mode);
}
