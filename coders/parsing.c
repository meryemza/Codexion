/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mezahir <mezahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 11:22:45 by mezahir           #+#    #+#             */
/*   Updated: 2026/07/12 10:07:25 by mezahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	valide_number(char *arg)
{
	int	i;

	i = 0;
	while ((arg[i] >= 9 && arg[i] <= 13) || arg[i] == ' ')
		i++;
	if (arg[i] == '-')
		return (1);
	else if (arg[i] == '+')
		i++;
	else if (arg[i] == '\0')
		return (1);
	if (!(arg[i] >= '0' && arg[i] <= '9'))
		return (1);
	while (arg[i] >= '0' && arg[i] <= '9')
		i++;
	while ((arg[i] >= 9 && arg[i] <= 13) || arg[i] == ' ')
		i++;
	if (arg[i] != '\0')
		return (1);
	return (0);
}

long long	ft_atoll(char *str)
{
	int			i;
	long long	nb;

	nb = 0;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = (nb * 10) + (str[i] - '0');
		i++;
	}
	return (nb);
}

int	valide_scheduler(char *arg)
{
	if (strcmp(arg, "fifo") == 0)
		return (0);
	else if (strcmp(arg, "edf") == 0)
		return (0);
	else
		return (1);
}

int	valide_args(int argc, char *argv[])
{
	int	i;

	i = 1;
	while (i < argc - 1)
	{
		if (valide_number(argv[i]) || ft_atoll(argv[i]) > INT_MAX)
		{
			printf("ERROR in argument %d: you must enter a valid number\n", i);
			return (1);
		}
		i++;
	}
	return (0);
}

int	parsing(int argc, char *argv[])
{
	if (argc != 9)
	{
		printf("you must enter 8 arguments exatly : 'number_of_coders' "
			"'time_to_burnout' 'time_to_compile' 'time_to_debug' "
			"'time_to_refactor' 'number_of_compiles_required' "
			"'dongle_cooldown' 'scheduler'\n");
		return (1);
	}
	else
	{
		if (valide_args(argc, argv))
			return (1);
		if (atoi(argv[1]) == 0)
		{
			printf("you must enter a number of coders greater than 0\n");
			return (1);
		}
		if (valide_scheduler(argv[argc - 1]))
		{
			printf("scheduler must be fifo or edf\n");
			return (1);
		}
		return (0);
	}
	return (0);
}
