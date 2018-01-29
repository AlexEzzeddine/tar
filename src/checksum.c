/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checksum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 19:59:29 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 19:59:41 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

void			calculate_checksum(t_tar_header *header)
{
	unsigned int	sum;
	size_t			i;
	char			*ptr;

	sum = 0;
	i = 0;
	memset(header->checksum, ' ', 8);
	ptr = (char *)header;
	while (i < sizeof(t_tar_header))
		sum += ptr[i++];
	sprintf(header->checksum, "%06o", sum);
}
