/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 18:58:40 by nweber            #+#    #+#             */
/*   Updated: 2025/09/11 19:02:14 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fill_env_array(t_list *env, char **array)
{
	int		index;
	char	*dup;

	index = 0;
	while (env)
	{
		dup = ft_strdup((char *)env->content);
		if (!dup)
			return (1);
		array[index] = dup;
		index++;
		env = env->next;
	}
	array[index] = NULL;
	return (0);
}

int	count_env_nodes(t_list *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

int	replace_node_kv(t_list *node, const char *kv_pair)
{
	char	*new_str;

	if (!node)
		return (1);
	new_str = ft_strdup(kv_pair);
	if (!new_str)
		return (1);
	free(node->content);
	node->content = new_str;
	return (0);
}

t_list	*find_key_node(t_list *env, const char *key, size_t key_len)
{
	char	*entry;

	while (env)
	{
		entry = (char *)env->content;
		if (entry && key_matches(entry, key, key_len))
			return (env);
		env = env->next;
	}
	return (NULL);
}
