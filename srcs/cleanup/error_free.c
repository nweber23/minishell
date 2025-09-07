/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:58:41 by nweber            #+#    #+#             */
/*   Updated: 2025/09/06 20:03:09 by nweber           ###   ########.fr       */
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
	int	i;

	if (shell->root)
	{
		free_binary(shell->root);
		shell->root = NULL;
	}
	ft_lstclear(&shell->tokens, free_token);
	shell->tokens = NULL;
	i = -1;
	if (shell->env_array)
	{
		while (shell->env_array[++i])
			free(shell->env_array[i]);
		free(shell->env_array);
		shell->env_array = NULL;
	}
	ft_lstclear(&shell->path, free);
	shell->path = NULL;
	if (shell->cwd)
		free(shell->cwd);
	shell->cwd = NULL;
	if (shell->input)
		free(shell->input);
	shell->input = NULL;
	if (shell->trimmed)
		free(shell->trimmed);
	shell->trimmed = NULL;
	if (shell->command_path)
		free(shell->command_path);
	shell->command_path = NULL;
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
