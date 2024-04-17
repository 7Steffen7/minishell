/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sparth <sparth@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 23:30:57 by sparth            #+#    #+#             */
/*   Updated: 2024/04/17 01:01:20 by sparth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_exit(char *str)
{
	printf("%s\n", str);
	exit (1);
}

void	clean_all_exit(t_data *data, int exit_code)
{
	free_everything(data);
	exit(exit_code);
}
