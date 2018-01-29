/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 19:59:47 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 20:00:07 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

char			*join_path(char *s1, char *s2)
{
	int		len1;
	char	*result;
	int		slash;

	slash = 1;
	len1 = strlen(s1);
	if (s1[len1 - 1] == '/')
		slash = 0;
	result = malloc(strlen(s1) + slash + strlen(s2) + 1);
	if (result)
	{
		strcpy(result, s1);
		if (slash)
			strcat(result, "/");
		strcat(result, s2);
	}
	return (result);
}
