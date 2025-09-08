/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:55:00 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/07 18:03:51 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	env_key_equals(const char *entry_kv, const char *target_key)
{
	size_t	entry_key_len;
	size_t	target_len;

	entry_key_len = 0;
	while (entry_kv[entry_key_len] && entry_kv[entry_key_len] != '=')
		entry_key_len++;
	target_len = ft_strlen(target_key);
	if (entry_key_len != target_len)
		return (0);
	return (ft_strncmp(entry_kv, target_key, entry_key_len) == 0);
}

static void	unlink_env_node(t_shell_data *shell,
		t_list *previous_node, t_list *current_node)
{
	t_list	*next_node;

	next_node = current_node->next;
	if (previous_node)
		previous_node->next = next_node;
	else
		shell->env = next_node;
	free(current_node->content);
	free(current_node);
}

static void	remove_key_once(t_shell_data *shell, const char *key_to_remove)
{
	t_list	*previous_node;
	t_list	*current_node;

	previous_node = NULL;
	current_node = shell->env;
	while (current_node)
	{
		if (env_key_equals((char *)current_node->content, key_to_remove))
		{
			unlink_env_node(shell, previous_node, current_node);
			return ;
		}
		previous_node = current_node;
		current_node = current_node->next;
	}
}

int	bi_unset(t_shell_data *shell, char **argument_vector)
{
	int	argument_index;

	if (!argument_vector)
		return (0);
	argument_index = 0;
	while (argument_vector[argument_index])
	{
		remove_key_once(shell, argument_vector[argument_index]);
		argument_index++;
	}
	return (0);
}
