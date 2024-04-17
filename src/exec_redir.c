/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sparth <sparth@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 23:49:01 by sparth            #+#    #+#             */
/*   Updated: 2024/04/17 02:11:09 by sparth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	input_redirect(t_node *node, t_data *data)
{
	int	fd_in;

	fd_in = open(node->infile, O_RDONLY);
	if (fd_in == -1)
	{
		if (access(node->infile, F_OK) == 0)
			ft_printf("minishell: %s: %s\n", node->infile, strerror(13));
		else
		{
			ft_printf("minishell: %s: %s\n", node->infile, strerror(2));
		}
		clean_all_exit(data, 1);
	}
	if (dup2(fd_in, STDIN_FILENO) == -1)
		clean_all_exit(data, 1);
	if (close(fd_in) == -1)
		clean_all_exit(data, 1);
	if (node->next)
		exec(node->next, data);
	else
		clean_all_exit(data, 0);
}

void	heredoc_exec(t_node *node, int fd[2])
{
	char	*line;

	while (1)
	{
		write(1, "here_doc> ", 10);
		line = get_next_line(STDIN_FILENO);
		if (ft_strncmp(node->limiter, line, ft_strlen(node->limiter)) == 0
			&& ft_strlen(node->limiter) + 1 == ft_strlen(line))
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		free(line);
		line = NULL;
	}
}

void	heredoc(t_node *node, t_data *data)
{
	int		fd[2];

	if (pipe(fd) == -1)
		clean_all_exit(data, 2);
	heredoc_exec(node, fd);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		clean_all_exit(data, 3);
	if (close(fd[0]) == -1 || close(fd[1]) == -1)
		clean_all_exit(data, 4);
	if (node->next)
		exec(node->next, data);
	else
		clean_all_exit(data, 0);
}

void	output_redirect(t_node *node, t_data *data)
{
	int	fd_out;

	fd_out = open(node->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out == -1)
	{
		if (access(node->outfile, F_OK) == 0)
			ft_printf("minishell: %s: %s\n", node->outfile, strerror(13));
		else
			ft_printf("minishell: %s: %s\n", node->outfile, strerror(2));
		clean_all_exit(data, 1);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
		clean_all_exit(data, 2);
	if (close(fd_out) == -1)
		clean_all_exit(data, 2);
	if (node->next)
		exec(node->next, data);
	else
		clean_all_exit(data, 0);
}

void	output_redirect_append(t_node *node, t_data *data)
{
	int	fd_out;

	fd_out = open(node->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		if (access(node->outfile, F_OK) == 0)
			ft_printf("minishell: %s: %s\n", node->outfile, strerror(13));
		else
			ft_printf("minishell: %s: %s\n", node->outfile, strerror(2));
		clean_all_exit(data, 1);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
		clean_all_exit(data, 2);
	if (close(fd_out) == -1)
		clean_all_exit(data, 2);
	if (node->next)
		exec(node->next, data);
	else
		clean_all_exit(data, 0);
}
