/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cd_buildin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sparth <sparth@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:59:23 by sparth            #+#    #+#             */
/*   Updated: 2024/04/17 02:01:00 by sparth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	change_pwds(t_data *data, bool flag)
{
	char	*temp;
	char	*cwd;

	cwd = getcwd(NULL, PATH_MAX);
	if (flag == 0)
	{
		temp = ft_strjoin("OLDPWD=", cwd);
		if (!temp)
		{
			printf("ft_strjoin failed\n");
			exit (1);
		}
	}
	if (flag == 1)
	{
		temp = ft_strjoin("PWD=", cwd);
		if (!temp)
		{
			printf("ft_strjoin failed\n");
			exit (1);
		}
	}
	free(cwd);
	export(temp, data);
	free(temp);
}

void	prep_dir_change(t_data *data, int flag, char *path)
{
	char	*cwd;

	if (path)
	{
		change_pwds(data, 0);
		if (chdir(path) == -1)
		{
			printf("chdir failed\n");
			exit (1);
		}
		change_pwds(data, 1);
	}
	else
	{
		change_pwds(data, 0);
		cwd = getcwd(NULL, PATH_MAX);
		string_cut(cwd, flag);
		free(cwd);
		change_pwds(data, 1);
	}
}

bool	switch_between_dir(t_data *data)
{
	char	*old_pwd;
	char	*temp;
	t_env	*temp_lst;

	temp_lst = data->env_list;
	while (temp_lst)
	{
		if (ft_strncmp(temp_lst->var_name, "OLDPWD", 7) == 0)
		{
			old_pwd = getcwd(NULL, PATH_MAX);
			if (chdir(temp_lst->var_str) == -1)
				print_exit("chdir failed");
			temp = ft_strjoin("OLDPWD=", old_pwd);
			free(old_pwd);
			if (!temp)
				print_exit("ft_strjoin failed");
			export(temp, data);
			free(temp);
			change_pwds(data, 1);
			return (1);
		}
		temp_lst = temp_lst->next;
	}
	printf("minishell: cd: OLDPWD not set");
	return (-1);
}

int	path_change(char *cmd, t_data *data)
{
	if (access(cmd, F_OK))
	{
		ft_printf("minishell: cd: %s: %s\n", cmd, strerror(2));
		return (-1);
	}
	prep_dir_change(data, 0, cmd);
	return (1);
}

int	look_4_cd(t_node *node, t_data *data)
{
	if (node->node_type == EXEC && node->command[0] && \
		ft_strncmp(node->command[0], "cd", ft_strlen(node->command[0]) + 1) == 0
		&& node->command[1] == NULL)
	{
		prep_dir_change(data, 2, NULL);
		return (1);
	}
	if (node->node_type == EXEC && node->command[0] && \
		ft_strncmp(node->command[0], "cd", ft_strlen(node->command[0]) + 1) == 0
		&& node->command[1] && node->command[2] == NULL)
	{
		if (ft_strncmp(node->command[1], ".", 3) == 0)
			return (1);
		else if (ft_strncmp(node->command[1], "..", 3) == 0)
			prep_dir_change(data, 0, NULL);
		else if (ft_strncmp(node->command[1], "~", 2) == 0)
			prep_dir_change(data, 2, NULL);
		else if (ft_strncmp(node->command[1], "-", 2) == 0)
			return (switch_between_dir(data));
		else
			return (path_change(node->command[1], data));
		return (1);
	}
	return (0);
}
