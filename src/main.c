/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 17:38:30 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 17:52:32 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

/*
** extract:
** second parameter = restore access time yes/no tar -p
** third parameter, print file names to stdout tar -v
*/

int	main(int ac, char **av)
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
