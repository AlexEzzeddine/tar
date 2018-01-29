#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <tar.h>
#include <utime.h>
#include <sys/time.h>
#include <sys/stat.h>

typedef struct	s_tar
{
	char	file_name[100];
	char	file_mode[8];	//octal  000644,space,null	//offset 100
	char	owner_id[8];	//octal
	char	group_id[8];
	char	file_size[12];	//octal
	char	last_mod[12];	//
	char	checksum[8];	// six digit octal with leading zeros, followed by a null, space
	char	file_type[1];			//offset 156
	char	name_of_link[100];
	char	ustar[6];
	char	ustar_ver[2];
	char	owner_name[32];
	char	group_name[32];
	char	dev_major[8];
	char	dev_minor[8];
	char	file_prefix[155];
}				t_tar;

void	*ft_memalloc(size_t size)
{
	unsigned char	*p;
	size_t			i;

	if ((p = malloc(size)) == NULL)
		return (NULL);
	i = 0;
	while (i < size)
	{
		p[i] = 0;
		i++;
	}
	return (p);
}

void	quit(int errnum, void *memory)
{
	if (memory != NULL)
		free(memory);
	printf("%s\n",strerror(errnum));
	exit(EXIT_FAILURE);
}

ssize_t	getfilesize(char *filename)
{
	int		fd;
	ssize_t	size;
	int		r;
	char	buf;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return (-1);
	size = 0;
	while ((r = read(fd, &buf, 1)) == 1)
		size++;
	close(fd);
	if (r == 0)
		return (size);
	else
		return (-1);
}

void	*getfilecontents(char *filename, size_t *size)
{
	ssize_t			r;
	int				fd;
	unsigned char	*mem;

	if ((int)(*size = getfilesize(filename)) == -1)
		quit(errno, NULL);
	if ((mem = ft_memalloc(*size + 1)) == NULL)
		quit(errno, NULL);
	if ((fd = open(filename, O_RDONLY)))
	{
		r = read(fd, mem, *size);
		close(fd);
		if (r == (ssize_t)*size)
			return (mem);
	}
	free(mem);
	quit(errno, NULL);
	return (NULL);
}

int	check_magic(t_tar *t)
{
	if (strncmp(t->ustar, (const char *)TMAGIC, TMAGLEN) != 0)
	{
		printf("%s: ", t->file_name);
		printf("error: bad tar file? ustar magic is incorrect\n");
		return (0);
	}
	if (strncmp(t->ustar_ver, TVERSION, TVERSLEN) != 0)
	{
		printf("%s: ", t->file_name);
		printf("error: bad tar file? ustar version does not match\n");
		return (0);
	}
	return (1);
}

int	is_valid_header(t_tar *t)
{
	long int	checksum;
	size_t		i;

	if (!check_magic(t))
		return (0);
	i = 0;
	checksum = 8 * 32;
	while (i < sizeof(t_tar))
	{
		checksum += ((unsigned char *)t)[i];
		i++;
		if (i == 148)
			i+= 8;
	}
	if (checksum != strtol(t->checksum, NULL, 8))
	{
		printf("%s: ", t->file_name);
		printf("error: bad tar file? checksum does not match\n");
		return(0);
	}
	return (1);
}

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
	return;
}

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


int	put_file_contents(char *filename, unsigned char *data, size_t size)
{
	int		fd;

	if ((fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644)) == -1)
		return (0);
	if ((write(fd, data, size)) == -1)
	{
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
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
	pad = ((((fs / 512) + 1 ) * 512) - fs);
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
	//add handling of other file types: fifo, char, block, symlink
	return ;
}

int	are_next_two_blocks_empty(char *data, size_t i, size_t size)
{
	size_t	j;
	size_t	sum;

	j = i + (512 * 2);
	sum = 0;

	if (j <= size)
	{
		while (i < j)
		{
			sum += data[i];
			i++;
		}
		if (sum == 0)
			return (1);
	}
	return (0);
}

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
		pad = ((((filesize / 512) + 1 ) * 512) - filesize);
		i += filesize;
		if (pad != 512)
			i += pad;
	}
	return ;
}

int	extract(char *file, int restore, int verbose)
{
	t_tar	*t;
	size_t	i;
	size_t	size;
	char	*data;

	if ((data = getfilecontents(file, &size)) == NULL)
	{
		printf("%s: error: could not read file\n", file);
		return (0);
	}
	i = 0;
	while (i < size)
	{
		t = (t_tar *)&data[i];
		if (is_valid_header(t))
			extract_file(t, &i, verbose);
		if (are_next_two_blocks_empty(data, i, size) == 1)
			break ;
	}
	restore_modtime(data, size, restore);
	free(data);
	if (i != size)
		return (0);
	return (1);
}

//////////////////////////////////////////////////////

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

int	count_digits(size_t n)
{
	int i;

	if (n == 0)
		return (1);
	i = 0;
	while (n)
	{
		n /= 10;
		i++;
	}
	return (i);
}

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
	static int	owner_length;
	static int	group_length;
	static int	size_length;

	st_mode = strtol(t->file_mode, NULL, 8);
	print_file_mode(t, st_mode);
	printf("  0 "); //lame
	if ((int)strlen(t->owner_name) > owner_length)
		owner_length = strlen(t->owner_name);
	if ((int)strlen(t->group_name) > group_length)
		group_length = strlen(t->group_name);
	printf("%-*s %-*s ", owner_length, t->owner_name, group_length, t->group_name);
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
		pad = ((((filesize / 512) + 1 ) * 512) - filesize);
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
		pad = ((((filesize / 512) + 1 ) * 512) - filesize);
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
		printf("invalid tar archive\n");
	free(data);
	return ;
}





int	main(int ac, char **av)
{
	if (ac == 2)
	{
		extract(av[1], 1, 1);	//second parameter = restore access time yes/no tar -p
								//third parameter, print file names to stdout tar -v
	}
	else if (ac == 3)
	{
		if ((av[1][0] == '-') && (av[1][1] == 't'))
			print(av[2], 1);
	}
	else


		printf("usage: ./untar <file>\n");

/*
		printf("file name: %s\n", a->file_name);
		printf(" owner id: %s\n", a->owner_id);
		printf(" group id: %s\n", a->group_id);
		printf("file size: %.12s\n", a->file_size);
		printf(" last mod: %s\n", a->last_mod);
		printf(" checksum: %s\n", a->checksum);
		printf("file type: %s\n", a->file_type);
		printf("link name: %s\n", a->name_of_link);
		printf("    ustar: %s\n", a->ustar);
		printf("ustar ver: %s\n", a->ustar_ver);
		printf("owner nam: %s\n", a->owner_name);
		printf("group nam: %s\n", a->group_name);
		printf("dev major: %s\n", a->dev_major);
		printf("dev minor: %s\n", a->dev_minor);
		printf("file pref: %s\n", a->file_prefix);
*/

	return (0);

}
