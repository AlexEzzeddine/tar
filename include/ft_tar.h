/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tar.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 17:21:22 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 20:27:22 by asarandi         ###   ########.fr       */
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
# include <err.h>
# include <pwd.h>
# include <grp.h>
# include <sys/types.h>
# include <dirent.h>

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

typedef	t_tar	t_tar_header;

typedef enum	e_tar_mode
{
	NONE,
	CREATE,
	EXTRACT,
	LIST
}				t_tar_mode;

typedef struct	s_tar_options
{
	t_tar_mode	mode;
	int			verbose;
	char		*archive_name;
	int			restore_permissions;
	char		**input_files;
	int			input_files_len;
}				t_tar_options;

char			*join_path(char *s1, char *s2);
char			check_enabled_modes(char c);
char			*make_file_name(t_tar *t);
char			*time_string(t_tar *t);
int				count_digits(size_t n);
int				extract(char *file, int restore, int verbose);
int				main(int argc, char **argv);
int				put_file_contents
					(char *filename, unsigned char *data, size_t size);
int				are_next_two_blocks_empty
					(char *data, size_t i, size_t size);
int				check_magic(t_tar *t);
int				is_valid_header(t_tar *t);
int				main(int ac, char **av);
ssize_t			getfilesize(char *filename);
t_tar_header	*create_header(char *filename);
void			calculate_checksum(t_tar_header *header);
void			check_input_files(void);
void			enable_mode(char c);
void			fill_header_device_info(t_tar_header *header, dev_t st_rdev);
void			fill_header_file_size(t_tar_header *header, off_t st_size);
void			fill_header_link_name(t_tar_header *header, char *filename);
void			fill_header_mod_time(t_tar_header *header, time_t mtime);
void			fill_header_name(t_tar_header *header, char *filename);
void			fill_header_owner_info
					(t_tar_header *header, uid_t uid, gid_t gid);
void			fill_header_type
					(t_tar_header *header, mode_t mode, char *filename);
void			ft_tar(void);
void			parse_options(int argc, char **argv);
void			tar_create(void);
void			*ft_memalloc(size_t size);
void			*getfilecontents(char *filename, size_t *size);
void			add_children_to_archive(FILE *archive, char *directory);
void			add_content_to_archive(FILE *archive, FILE *file);
void			add_file_to_archive(FILE *archive, char *filename);
void			add_header_to_archive(t_tar_header *header, FILE *file);
void			extract_file(t_tar *t, size_t *i, int verbose);
void			log_file(char *file);
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
