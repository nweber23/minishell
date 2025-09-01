/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:58:41 by nweber            #+#    #+#             */
/*   Updated: 2025/09/01 10:13:05 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	if (token)
	{
		free(token->value);
		free(token);
	}
}

void	free_env(t_list *env)
{
	t_list	*next;

	while (env)
	{
		next = env->next;
		free(env->content);
		free(env);
		env = next;
	}
}

void	free_shell(t_shell_data *shell)
{
	if (shell)
	{
		ft_lstclear(&shell->tokens, free_token);
		free(shell->input);
		free(shell);
	}
}

void	free_logic_tree(void *root)
{
	t_node_type	node;

	if (!root)
		return ;
	node = *(t_node_type *)root;
	if (node == N_AND)
	{
		free_logic_tree(((t_and_point *)root)->left);
		free_logic_tree(((t_and_point *)root)->right);
	}
	if (node == N_OR)
	{
		free_logic_tree(((t_or_point *)root)->left);
		free_logic_tree(((t_or_point *)root)->right);
	}
	if (node != N_AND && node != N_OR)
		free_binary((t_pipe *)root);
	else if (node == N_AND || node == N_OR)
		free(root);
}
