/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 10:32:06 by nweber            #+#    #+#             */
/*   Updated: 2025/09/05 17:17:44 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*build_binary_tree(t_shell_data *shell, t_list	*tokens)
{
	t_list	*temp;
	void	*root;

	root = NULL;
	temp = tokens;
	while (token_check(temp))
	{
		root = insert_node(shell, root, temp);
		if (((t_token *)temp)->type != PIPE)
			temp = remove_direct(temp);
		else
			temp = remove_goto_next(temp);
	}
	return (root);
}

void	*insert_node(t_shell_data *shell, void *root, t_list *temp)
{
	t_pipe	*pipe;
	t_list	*current;

	if (!root)
		root = create_execution(shell, temp);
	if (is_parenthesis(temp->next))
	{
		current = create_new_sublist(temp->next);
		pipe = create_pipe(shell, root, build_tree(shell, current));
		clear_sublist(current);
	}
	else
		pipe = create_pipe(shell, root, create_execution(shell, temp->next));
	return (pipe);
}

void	*create_execution(t_shell_data *shell, t_list *tokens)
{
	t_exec	*exec;
	t_list	*current;

	if (!tokens)
		return (NULL);
	exec = malloc(sizeof(t_exec));
	if (!exec)
		error_malloc("create_execution", shell);
	exec->type = N_EXEC;
	exec->argv = NULL;
	exec->command = NULL;
	exec->infile = NULL;
	exec->outfile = NULL;
	get_infiles(shell, tokens, &exec->infile);
	get_outfiles(shell, tokens, &exec->outfile);
	current = get_name(tokens);
	if (current)
	{
		exec->command = ((t_token *)current)->value;
		exec->argv = get_argv(shell, tokens);
	}
	return (exec);
}
