/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sparth <sparth@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:46:31 by aweissha          #+#    #+#             */
/*   Updated: 2024/04/16 22:48:46 by sparth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env	*find_lowest_var(t_env *env_list)
{
	t_env	*lowest;

	lowest = env_list;
	while (env_list != NULL)
	{
		if (ft_strcmp(lowest->var_name, env_list->var_name) > 0)
			lowest = env_list;
		env_list = env_list->next;
	}
	return (lowest);
}

t_env	*find_highest_var(t_env *env_list)
{
	t_env	*highest;

	highest = env_list;
	while (env_list != NULL)
	{
		if (ft_strcmp(highest->var_name, env_list->var_name) < 0)
			highest = env_list;
		env_list = env_list->next;
	}
	return (highest);
}

t_env	*find_next_higher(t_env *var, t_env *env_list)
{
	t_env	*next_higher;
	t_env	*highest;

	highest = find_highest_var(env_list);
	if (highest == var)
		return (NULL);
	else
		next_higher = highest;
	while (env_list != NULL)
	{
		if (ft_strcmp(env_list->var_name, var->var_name) > 0
			&& ft_strcmp(next_higher->var_name, env_list->var_name) > 0)
			next_higher = env_list;
		env_list = env_list->next;
	}
	return (next_higher);
}
