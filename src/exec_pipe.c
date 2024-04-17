/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sparth <sparth@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 21:26:46 by sparth            #+#    #+#             */
/*   Updated: 2024/04/17 02:06:13 by sparth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	left_pipe(pid_t pid_left, int fd[2], t_node *node, t_data *data)
{
	if (pid_left == -1)
		clean_all_exit(data, 1);
	if (pid_left == 0)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			clean_all_exit(data, 1);
		if (close(fd[0]) == -1 || close(fd[1]) == -1)
			clean_all_exit(data, 1);
		exec(node->left, data);
	}
}

void	right_pipe(pid_t pid_right, int fd[2], t_node *node, t_data *data)
{
	if (pid_right == -1)
		clean_all_exit(data, 1);
	if (pid_right == 0)
	{
		if (dup2(fd[0], STDIN_FILENO) == -1)
			clean_all_exit(data, 1);
		if (close(fd[0]) == -1 || close(fd[1]) == -1)
			clean_all_exit(data, 1);
		exec(node->right, data);
	}
}

void	piping(t_node *node, t_data *data)
{
	int		fd[2];
	int		wpidstatus;
	pid_t	pid_left;
	pid_t	pid_right;

	if (pipe(fd) == -1)
		clean_all_exit(data, 1);
	pid_left = fork();
	left_pipe(pid_left, fd, node, data);
	pid_right = fork();
	right_pipe(pid_right, fd, node, data);
	if (pid_right != 0 && pid_left != 0)
	{
		if (close(fd[0]) == -1 || close(fd[1]) == -1)
			clean_all_exit(data, 39);
	}
	waitpid(pid_right, &wpidstatus, 0);
	waitpid(pid_left, NULL, 0);
	free_everything(data);
	if (WIFEXITED(wpidstatus))
		exit (WEXITSTATUS(wpidstatus));
}
