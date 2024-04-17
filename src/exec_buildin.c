/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_buildin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sparth <sparth@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 20:28:56 by sparth            #+#    #+#             */
/*   Updated: 2024/04/16 22:52:23 by sparth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	check_if_buildin(t_node *node, t_data *data)
{
	if (ft_strncmp (node->command[0], "env", 4) == 0)
	{
		env(data);
		free_everything(data);
		exit (0);
	}
	if ((node->command[0][0] == 'e' || node->command[0][0] == 'E')
		&& (node->command[0][1] == 'c' || node->command[0][1] == 'C')
		&& (node->command[0][2] == 'h' || node->command[0][2] == 'H')
		&& (node->command[0][3] == 'o' || node->command[0][3] == 'O')
		&& node->command[0][4] == '\0')
		echo(node, data);
	if (ft_strncmp (node->command[0], "pwd", 4) == 0)
		pwd(data);
	if (ft_strncmp (node->command[0], "exit", 5) == 0
		|| ft_strncmp (node->command[0], "cd", 3) == 0
		|| ft_strncmp (node->command[0], "unset", 6) == 0
		|| ft_strncmp (node->command[0], "export", 7) == 0)
	{
		free_everything(data);
		exit (0);
	}
}

int	look_4_buildins(t_node *node, t_data *data)
{
	int	cd_value;
	int	ex_value;

	if (look_4_exit(node, data))
		return (-1);
	if (look_4_unset(node, data))
		return (1);
	ex_value = look_4_export(node, data);
	if (ex_value)
		return (ex_value);
	cd_value = look_4_cd(node, data);
	if (cd_value)
		return (cd_value);
	return (0);
}
