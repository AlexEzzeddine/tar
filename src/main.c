/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezzeddi <aezzeddi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 18:40:35 by aezzeddi          #+#    #+#             */
/*   Updated: 2018/01/28 20:02:08 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

t_tar_options g_tar_options = {.input_files_len = 0};

int		main(int argc, char **argv)
{
	parse_options(argc, argv);
	ft_tar();
	return (0);
}
