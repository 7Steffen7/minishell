/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sparth <sparth@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 11:07:54 by aweissha          #+#    #+#             */
/*   Updated: 2024/04/17 01:31:26 by sparth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void	test_parse_tree(t_node *node)
// {
// 	int	i;

// 	if (node == NULL)
// 	{
// 		printf("end of parse_tree reched\n");
// 		return ;
// 	}
// 	if (node->node_type == PIPE)
// 	{
// 		printf("hello from PIPE node\n");
// 		test_parse_tree(node->left);
// 		test_parse_tree(node->right);
// 	}
// 	else if (node->node_type == REDINPT)
// 	{
// 		printf("hello from REDINPT node\n");
// 		test_parse_tree(node->next);
// 	}
// 	else if (node->node_type == REDOUT)
// 	{
// 		printf("hello from REDOUT node\n");
// 		test_parse_tree(node->next);
// 	}
// 	else if (node->node_type == REDAPPND)
// 	{
// 		printf("hello from REDAPPND node\n");
// 		test_parse_tree(node->next);
// 	}
// 	else if (node->node_type == HEREDOC)
// 	{
// 		printf("hello from HEREDOC node\n");
// 		test_parse_tree(node->next);
// 	}
// 	else if (node->node_type == EXEC)
// 	{
// 		printf("hello from EXEC node. Here comes the command:");
// 		i = 0;
// 		while (node->command[i] != NULL)
// 		{
// 			printf("%s ", node->command[i]);
// 			i++;
// 		}
// 		printf("\n");
// 	}
// }

void	test_env_list(t_data *data)
{
	t_env	*env_list;

	env_list = data->env_list;
	while (env_list != NULL)
	{
		printf("%s=%s\n", env_list->var_name, env_list->var_str);
		env_list = env_list->next;
	}
}

void	read_line(t_data *data)
{
	char	*input;

	pre_rl_signals();
	input = readline("\x1b[32mMinishell $> \x1b[0m");
	if (!input)
	{
		free_everything(data);
		printf("exit\n");
		exit (0);
	}
	signal(SIGINT, SIG_IGN);
	add_history(input);
	lexer(input, data);
	free(input);
}

int	main(int argc, char **argv, char **env)
{
	t_data	*data;

	data = init_data(argc, argv, env);
	create_env_list(data);
	while (1)
	{
		read_line(data);
		if (syntax_check(data) == 1)
		{
			free_token_list(data->token_list);
			data->token_list = NULL;
			continue ;
		}
		expander(data);
		data->parse_tree = parse_pipe(data->token_list, data);
		data->token_list = NULL;
		data->last_exit_code = pre_exec(data->parse_tree, data);
		free_parse_tree(data->parse_tree);
		data->parse_tree = NULL;
	}
	return (0);
}

/*
To do:
- check eval sheet for edge cases
- clean history
- check for memory leaks
- Norminette
...
*/
