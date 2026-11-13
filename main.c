/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaestre <jmaestre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 12:07:56 by username          #+#    #+#             */
/*   Updated: 2026/11/13 13:13:31 by jmaestre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	parse_args(char *arg)
{
	printf("%s\n", arg);
	// Viene dentro de comillas
	if (ft_strchr(arg, ' '))
	{
		printf("hay comillas\n");
	}
	return (0);
}

int	check_args(int argc, char **argv)
{
	int	i;

	if (argc == 1)
		return (0);
	else
	{
		i = 0;
		while (*argv)
		{
			parse_args(*argv);
			argv++;
		}
	}
	return (0);
}

int	main(int argc, char **argv)
{
	int	i;

	i = check_args(argc, argv);
	if (i < 0)
	{
		ft_write_error();
		return (-1);
	}
	return (0);
}
