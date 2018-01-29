/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezzeddi <aezzeddi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/28 18:40:35 by aezzeddi          #+#    #+#             */
/*   Updated: 2018/01/28 21:33:30 by asarandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_tar.h"

t_tar_options g_tar_options = { NONE, 0, NULL, 0, NULL, 0};

void	show_usage(void)
{
	printf("usage: ./ft_tar	[command] [option] <input_file(s)>\n");
	printf("\tcommands (required):\n");
	printf("\t\t-x: extract\n");
	printf("\t\t-t: list\n");
	printf("\t\t-c: create\n");
	printf("\toptions:\n");
	printf("\t\t-f: file\n");
	printf("\t\t-p: preserve permissions\n");
	printf("\t\t-v: verbose\n");
	return ;
}

void	ft_tar(void)
{
	t_tar_mode mode;

	mode = g_tar_options.mode;
	if (mode == NONE)
	{
		show_usage();
		exit(1);
	}
	else if (mode == CREATE)
		tar_create();
	else if (mode == EXTRACT)
		extract(g_tar_options.archive_name, \
				g_tar_options.restore_permissions, g_tar_options.verbose);
	else if (mode == LIST)
		print(g_tar_options.archive_name, g_tar_options.verbose);
}

int		main(int argc, char **argv)
{
	parse_options(argc, argv);
	ft_tar();
	return (0);
}
