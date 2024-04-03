/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sparth <sparth@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 11:07:54 by aweissha          #+#    #+#             */
/*   Updated: 2024/04/03 16:58:40 by sparth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	test_parse_tree(t_node *node)
{
	if (node->node_type == PIPE)
	{
		printf("hello from PIPE node\n");
		test_parse_tree(node->left);
		test_parse_tree(node->right);
	}
	else if (node->node_type == REDINPT)
	{
		printf("hello from REDINPT node\n");
		test_parse_tree(node->next);
	}
	else if (node->node_type == REDOUT)
	{
		printf("hello from REDOUT node\n");
		test_parse_tree(node->next);
	}
	else if (node->node_type == REDAPPND)
	{
		printf("hello from REDAPPND node\n");
		test_parse_tree(node->next);
	}
	else if (node->node_type == HEREDOC)
	{
		printf("hello from HEREDOC node\n");
		test_parse_tree(node->next);
	}
	else if (node->node_type == EXEC)
	{
		printf("hello from EXEC node. Here comes the command:");
		int i = 0;
		while (node->command[i] != NULL)
		{
			printf("%s ", node->command[i]);
			i++;
		}
		printf("\n");
	}
}

int	main(int argc, char **argv, char **env)
{
	t_data	*data;
	t_token	*tmp;
	char	*input;

	while (1)
	{
		input = readline("Minishell $> ");
		// if (ft_strncmp(input, "exit", ft_strlen(input) + 1) == 0)
		// {
		// 	//free everything
		// 	break;
		// }
		// check for open quotes -> if open quotes abort
		add_history(input);
		data = init_data(argc, argv, env);
		lexer(input, data);

		tmp = data->token_list;
		while (tmp != NULL)
		{
			// printf("type: %u\ntoken_str: %s\n", tmp->token_type, tmp->token_str);
			tmp = tmp->next;
		}
		data->parse_tree = parse_pipe(data->token_list);
		test_parse_tree(data->parse_tree);
		pre_exec(data->parse_tree);
	}
	return (0);
}

// cc test.c error.c free.c handle_quotes.c init.c lexer.c parse_utils.c parser.c utils.c token_list_utils.c ../libft/libft.a -lreadline