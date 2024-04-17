/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sparth <sparth@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 20:03:16 by sparth            #+#    #+#             */
/*   Updated: 2024/04/17 01:50:42 by sparth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	check_exit_syntax(char *cmd)
{
	int	i;
	int	operator_count;

	i = 0;
	operator_count = 0;
	while (cmd[i])
	{
		if (cmd[i] == '-' || cmd[i] == '+')
		{
			operator_count++;
			if (operator_count > 1)
				return (false);
		}
		else if (cmd[i] == '\'' || cmd[i] == '"')
			return (false);
		else if (!ft_isdigit(cmd[i]))
			return (false);
		i++;
	}
	return (true);
}

void	check_invalid_exit(t_node *node, t_data *data)
{
	if (!check_exit_syntax(node->command[1]))
	{
		ft_printf("minishell: exit: %s", node->command[1]);
		ft_printf(": numeric argument required\n");
		clean_all_exit(data, 255);
	}
	else
	{
		printf("exit\n");
		ft_printf("minishell: exit: too many arguments\n");
	}
}

bool	look_4_exit(t_node *node, t_data *data)
{
	if (node->node_type == EXEC && node->command[0] != NULL
		&& ft_strncmp(node->command[0], "exit", 5) == 0)
	{
		if (node->command[1] == NULL)
		{
			printf("exit\n");
			clean_all_exit(data, 0);
		}
		else if (node->command[1] && node->command[2] == NULL)
		{
			if (!check_exit_syntax(node->command[1]))
			{
				ft_printf("minishell: exit: %s", node->command[1]);
				ft_printf(": numeric argument required\n");
				clean_all_exit(data, 255);
			}
			exit (ft_atoi(node->command[1]));
		}
		else if (node->command[1] && node->command[2])
		{
			check_invalid_exit(node, data);
			return (1);
		}
	}
	return (0);
}

int	exit_code(int wpidstatus)
{
	if (WIFEXITED(wpidstatus))
		return (WEXITSTATUS(wpidstatus));
	if (WIFSIGNALED(wpidstatus))
	{
		if (128 + WTERMSIG(wpidstatus) == 131)
			write (2, "Quit: 3", 7);
		write(2, "\n", 1);
		return (128 + WTERMSIG(wpidstatus));
	}
	return (86);
}

void	exec_exit(char	*path, t_node *node, t_data *data)
{
	struct stat	dir_check;

	if (path && access((path), X_OK) && access((path), W_OK) == 0)
	{
		free(path);
		clean_all_exit(data, 126);
	}
	if (path && stat(path, &dir_check) == 0)
	{
		if (S_ISDIR(dir_check.st_mode))
		{
			ft_printf("minishell: %s: is a directory\n", node->command[0]);
			clean_all_exit(data, 126);
		}
	}
	free(path);
	if (strncmp(node->command[0], "./", 2) == 0
		&& access((node->command[0]), F_OK) == 0)
		clean_all_exit(data, 126);
	clean_all_exit(data, 127);
}
