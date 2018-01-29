/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 17:29:04 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 21:09:21 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

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
			i += 8;
	}
	if (checksum != strtol(t->checksum, NULL, 8))
	{
		printf("error: bad tar file? checksum does not match\n");
		return (0);
	}
	return (1);
}
