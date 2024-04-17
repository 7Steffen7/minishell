/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sparth <sparth@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 20:16:26 by sparth            #+#    #+#             */
/*   Updated: 2024/04/17 01:53:20 by sparth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	*free_null(char *del)
{
	free(del);
	return (NULL);
}

void	path_error_message(char *error)
{
	if (strncmp(error, "/", 1) == 0)
		ft_printf("%s%s: %s\n", "minishell: ", error, strerror(2));
	else if (strncmp(error, "./", 2) == 0 && access(error, F_OK) == 0)
		ft_printf("%s%s: %s\n", "minishell: ", error, strerror(13));
	else if (strncmp(error, "./", 2) == 0)
		ft_printf("%s%s: %s\n", "minishell: ", error, strerror(2));
	else
		ft_printf("%s%s: %s\n", "minishell: ", error, "command not found");
}

char	*get_env(t_data *data)
{
	t_env	*env_temp;

	env_temp = data->env_list;
	while (env_temp)
	{
		if (ft_strncmp(env_temp->var_name, "PATH", 4) == 0)
			return (env_temp->var_str);
		env_temp = env_temp->next;
	}
	return (NULL);
}

void	string_cut_chdir(char *s, int char_count)
{
	int		i;
	char	cut[PATH_MAX];

	i = 0;
	while (i < char_count)
	{
		cut[i] = s[i];
		i++;
	}
	cut[i] = '\0';
	if (chdir(cut) == -1)
	{
		printf("chdir failed\n");
		exit (1);
	}
}

void	string_cut(char *s, int flag)
{
	int		slash_count;
	int		char_count;

	slash_count = 0;
	char_count = 0;
	while (s[char_count])
	{
		if (s[char_count] == '/')
			slash_count++;
		char_count++;
	}
	char_count = 0;
	if (slash_count <= 1)
		return ;
	while (slash_count)
	{
		if (slash_count - flag == 0)
			break ;
		if (s[char_count] == '/')
			slash_count--;
		char_count++;
	}
	string_cut_chdir(s, char_count);
}
