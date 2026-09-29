/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mezahir <mezahir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 21:22:54 by mezahir           #+#    #+#             */
/*   Updated: 2026/07/12 09:26:05 by mezahir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char *argv[])
{
	t_data	data;

	if (parsing(argc, argv))
		return (1);
	if (init_valide_data(argv, &data))
		return (1);
	return (0);
}
