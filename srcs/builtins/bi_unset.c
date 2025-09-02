
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:55:00 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 10:55:00 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int key_match(const char *kv, const char *key)
{
	size_t  name_len;

	name_len = 0;
	while (kv[name_len] && kv[name_len] != '=')
		name_len++;
	if (ft_strlen(key) != name_len)
		return (0);
	if (ft_strncmp(kv, key, name_len) == 0)
		return (1);
	return (0);
}

int bi_unset(t_shell_data *shell, char **args)
{
	t_list  *previous_node;
	t_list  *current_node;
	t_list  *next_node;
	int	 arg_index;

	if (!args)
		return (0);
	arg_index = 0;
	while (args[arg_index])
	{
		previous_node = NULL;
		current_node = shell->env;
		while (current_node)
		{
			next_node = current_node->next;
			if (key_match((char *)current_node->content, args[arg_index]))
			{
				if (previous_node)
					previous_node->next = next_node;
				else
					shell->env = next_node;
				free(current_node->content);
				free(current_node);
				break ;
			}
			previous_node = current_node;
			current_node = next_node;
		}
		arg_index++;
	}
	return (0);
}
