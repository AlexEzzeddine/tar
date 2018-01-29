/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tar.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 17:21:22 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 18:09:24 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TAR_H
# define FT_TAR_H

# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <errno.h>
# include <stdlib.h>
# include <string.h>
# include <tar.h>
# include <utime.h>
# include <sys/time.h>
# include <sys/stat.h>

typedef struct	s_tar
{
	char	file_name[100];
	char	file_mode[8];
	char	owner_id[8];
	char	group_id[8];
	char	file_size[12];
	char	last_mod[12];
	char	checksum[8];
	char	file_type[1];
	char	name_of_link[100];
	char	ustar[6];
	char	ustar_ver[2];
	char	owner_name[32];
	char	group_name[32];
	char	dev_major[8];
	char	dev_minor[8];
	char	file_prefix[155];
}				t_tar;

char			*make_file_name(t_tar *t);
char			*time_string(t_tar *t);
int				are_next_two_blocks_empty(char *data, size_t i, size_t size);
int				check_magic(t_tar *t);
int				count_digits(size_t n);
int				extract(char *file, int restore, int verbose);
int				is_valid_header(t_tar *t);
int				put_file_contents
					(char *filename, unsigned char *data, size_t size);
ssize_t			getfilesize(char *filename);
void			*ft_memalloc(size_t size);
void			*getfilecontents(char *filename, size_t *size);
void			extract_file(t_tar *t, size_t *i, int verbose);
void			print(char *file, int verbose);
void			print_details(char *filename, t_tar *t, size_t filesize);
void			print_entry_type(t_tar *t);
void			print_file_mode(t_tar *t, unsigned long st_mode);
void			print_filename(char *filename, int verbose);
void			print_long(char *data, size_t size);
void			print_permissions(unsigned long st_mode);
void			print_short(char *data, size_t size);
void			quit(int errnum, void *memory);
void			recreate_directory(t_tar *t, size_t *i, int verbose);
void			recreate_file(t_tar *t, size_t *i, int verbose);
void			restore_modtime(char *data, size_t size, int restore);
void			set_modtime(char *filename, t_tar *t);

#endif
