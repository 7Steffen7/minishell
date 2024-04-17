/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sparth <sparth@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:52:02 by sparth            #+#    #+#             */
/*   Updated: 2024/04/16 22:51:36 by sparth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	execution(t_node *node, t_data *data)
{
	char	*path;

	check_if_buildin(node, data);
	path = path_check(node->command[0], get_env(data));
	if (!path)
		path_error_message(node->command[0]);
	if (path)
		execve(path, node->command, NULL);
	exec_exit(path, node, data);
}

void	exec(t_node *node, t_data *data)
{
	if (node->node_type == PIPE)
		piping(node, data);
	else if (node->node_type == REDINPT)
		input_redirect(node, data);
	else if (node->node_type == REDOUT)
		output_redirect(node, data);
	else if (node->node_type == REDAPPND)
		output_redirect_append(node, data);
	else if (node->node_type == HEREDOC)
		heredoc(node, data);
	else if (node->node_type == EXEC)
		execution(node, data);
}

int	pre_exec(t_node *node, t_data *data)
{
	pid_t	pid;
	int		wpidstatus;
	int		r_value;

	if (node == NULL)
		return (0);
	r_value = look_4_buildins(node, data);
	if (r_value == 1)
		return (0);
	if (r_value == -1)
		return (1);
	pid = fork();
	if (pid == -1)
		exit (1);
	if (pid == 0)
	{
		signal(SIGINT, sig_child_int);
		signal(SIGQUIT, sig_child_quit);
		exec(node, data);
	}
	waitpid(pid, &wpidstatus, 0);
	return (exit_code(wpidstatus));
}
