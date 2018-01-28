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
		printf("error: bad tar file? ustar magic is incorrect\n");
		return (0);
	}
	if (strncmp(t->ustar_ver, TVERSION, TVERSLEN) != 0)
	{
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
		printf("error: bad tar file? checksum does not match\n");
		return(0);
	}
	return (1);
}

char	*make_file_name(t_tar *t)
{
	size_t	length;
	char	*data;

	length = strlen(t->file_prefix) + strlen(t->file_name) + 1;
	data = ft_memalloc(length + 1);
	data = strcat(data, t->file_prefix);
	data = strcat(data, t->file_name);
	return (data);
}

void	recreate_modtime(char *filename, t_tar *t)
{
	struct timeval *times[2];

	times = ft_memalloc(sizeof(struct timeval) * 2);

	times[0].tv_sec = strtol(t->last_mod, NULL, 8);
	times[0].tv_usec = 0;
	times[1].tv_sec = strtol(t->last_mod, NULL, 8);
	times[1].tv_usec = 0;

	printf("         filetime: %lu\n", strtol(t->last_mod, NULL, 8));
	printf("setting file time: %d\n", utimes(filename, times));
	return ;
}

void	recreate_directory(t_tar *t, size_t *i, int option)
{
	char	*filename;

	filename = make_file_name(t);
	if (mkdir(filename, strtol(t->file_mode, NULL, 8)) == -1)
		perror(strerror(errno));
	else
		chmod(filename, strtol(t->file_mode, NULL, 8));
	if (option == 1) //restore original modification time
		recreate_modtime(filename, t);
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


void	recreate_file(t_tar *t, size_t *i, int option)
{
	char	*fn;
	size_t	fs;

	fn = make_file_name(t);
	fs = strtol(t->file_size, NULL, 8);
	if (put_file_contents(fn, &((unsigned char *)t)[512], fs) == 0)
		perror(strerror(errno));
	else
		chmod(fn, strtol(t->file_mode, NULL, 8));
	if (option == 1)
		recreate_modtime(fn, t);
	free(fn);
	*i += 512;
	*i += fs + ((((fs / 512) + 1 ) * 512) - fs);
	return ;
}


void	extract_file(t_tar *t, size_t *i, int option)
{
	if (t->file_type[0] == DIRTYPE)
		recreate_directory(t, i, option);
	else if ((t->file_type[0] == REGTYPE) || (t->file_type[0] == AREGTYPE))
		recreate_file(t, i, option);
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

int	extract(char *file)
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
			extract_file(t, &i, 1);
		if (are_next_two_blocks_empty(data, i, size) == 1)
			break ;
	}
	free(data);
	if (i != size)
		return (0);
	return (1);
}


int	main(int ac, char **av)
{
//	printf("size of header is:  %lu\n", sizeof(t_tar));

	if (ac == 2)
	{
		extract(av[1]);
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
