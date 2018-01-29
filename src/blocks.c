/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blocks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 17:36:00 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 17:45:42 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

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
