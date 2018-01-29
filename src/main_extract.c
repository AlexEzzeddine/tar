/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_extract.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 17:38:30 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 20:23:18 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

/*
** extract (-x):
** first parameter = file name of archive
** second parameter = restore access time yes/no tar -p (always 1 ?)
** third parameter, print file names to stdout tar (-xv)
**
** print (-t):
** first parameter = file name of archive
** second parameter = verbose, 1 = long list print (-tv), 0 = short print
*/

int	main_abc(int ac, char **av)
{
	if (ac == 2)
	{
		extract(av[1], 1, 1);
	}
	else if (ac == 3)
	{
		if ((av[1][0] == '-') && (av[1][1] == 't'))
			print(av[2], 1);
	}
	else
		printf("usage: ./untar <file>\n");
	return (0);
}
