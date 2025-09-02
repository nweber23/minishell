
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:55:00 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/26 10:55:00 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	valid_key(const char *s)
{
	int	i;

	if (!s || !(ft_isalpha(*s) || *s == '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static void set_env_kv(t_shell_data *shell, const char *key_value_pair)
{
	t_list  *node;
	size_t  key_length;

	key_length = 0;
	while (key_value_pair[key_length] && key_value_pair[key_length] != '=')
		key_length++;
	node = shell->env;
	while (node)
	{
		if (ft_strncmp((char *)node->content, key_value_pair, key_length) == 0
			&& ((char *)node->content)[key_length] == '=')
		{
			free(node->content);
			node->content = ft_strdup(key_value_pair);
			return ;
		}
		node = node->next;
	}
	ft_lstadd_back(&shell->env, ft_lstnew(ft_strdup(key_value_pair)));
}

int bi_export(t_shell_data *shell, char **args)
{
	int arg_index;

	if (!args || !args[0])
		return (bi_env(shell));
	arg_index = 0;
	while (args[arg_index])
	{
		if (!valid_key(args[arg_index]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[arg_index], 2);
			ft_putendl_fd("': not a valid identifier", 2);
		}
		else
			set_env_kv(shell, args[arg_index]);
		arg_index++;
	}
	return (0);
}
