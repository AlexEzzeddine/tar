/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asarandi <asarandi@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 19:59:05 by asarandi          #+#    #+#             */
/*   Updated: 2018/01/28 20:25:59 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

extern t_tar_options	g_tar_options;

void			parse_options(int argc, char **argv)
{
	char c;

	while ((c = getopt(argc, argv, "xctvpf:")) != -1)
	{
		if (c == 'x' || c == 'c' || c == 't')
			enable_mode(c);
		else if (c == 'v')
			g_tar_options.verbose = 1;
		else if (c == 'p')
			g_tar_options.restore_permissions = 1;
		else if (c == 'f')
			g_tar_options.archive_name = strdup(optarg);
		else
			exit(1);
	}
	if (optind < argc)
	{
		g_tar_options.input_files = argv + optind;
		g_tar_options.input_files_len = argc - optind;
	}
}
