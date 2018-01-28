#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <tar.h>

typedef struct tar_header
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
}	t_header;

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



int	main(int ac, char **av)
{
	t_header	*a;
	printf("size of header is:  %lu\n", sizeof(t_header));

	if (ac == 2)
	{
		size_t	size;
		char	*data = getfilecontents(av[1], &size);
		if (data == NULL)
		{
			printf("could not read file %s\n", av[1]);
			return (0);
		}
		a = (struct tar_header *)data;

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




		free(data);


		
	
		


		

	}





	return (0);
}
