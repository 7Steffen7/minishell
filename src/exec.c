/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sparth <sparth@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:52:02 by sparth            #+#    #+#             */
/*   Updated: 2024/03/25 20:00:48 by sparth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	input_redirect(t_node *node)
{
	int	fd_in;

	printf("infile: %s\n", node->infile);
	fd_in = open(node->infile, O_RDONLY);
	if (fd_in == -1)
	{
		printf("error opening infile\n");
		exit(1);
	}
	close(fd_in);
}

void	output_redirect(t_node *node)
{
	int fd_out;
	
	printf("outfile: %s\n", node->outfile);
	fd_out = open(node->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out == -1)
	{
		printf("error opening outfile\n");
		exit (1);
	}
	close (fd_out);
}

void	output_redirect_append(t_node *node)
{
	int fd_out;
	
	printf("outfile_append: %s\n", node->outfile);
	fd_out = open(node->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		printf("error opening outfile\n");
		exit (1);
	}
	close (fd_out);
}

void piping(t_node *node)
{
		int fd[2];
		pid_t	pid_left;
		pid_t	pid_right;
		
		if (pipe(fd) == -1)
			exit (1);
		pid_left = fork();
		if (pid_left == -1)
			exit (1);
		if (pid_left == 0)
			exec(node->left);
		pid_right = fork();
		if (pid_right == -1)
			exit (1);
		if (pid_right == 0)
			exec(node->right);
		if (close(fd[0]) == -1 || close(fd[1]) == -1)
			exit (1);
		waitpid(pid_left, NULL, 0);
		waitpid(pid_right, NULL, 0);
}

void	exec(t_node *node)
{
	if (node->node_type == PIPE)
	{
		piping(node);
		// exec(node->left);
		// exec(node->right);
	}
	else if (node->node_type == REDINPT)
	{
		input_redirect(node);
		exec(node->next);
	}
	else if (node->node_type == REDOUT)
	{
		output_redirect(node);
		exec(node->next);
	}
	else if (node->node_type == REDAPPND)
	{
		output_redirect_append(node);
		exec(node->next);
	}
	else if (node->node_type == HEREDOC)
	{
		exec(node->next);
	}
	else if (node->node_type == EXEC)
	{
		
	}
}