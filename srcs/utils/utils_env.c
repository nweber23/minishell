/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 11:00:41 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/01 09:53:51 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	key_matches(const char *entry, const char *key, size_t key_len)
{
	if (ft_strncmp(entry, key, key_len) != 0)
		return (0);
	if (entry[key_len] != '=')
		return (0);
	return (1);
}

static t_list	*find_key_node(t_list *env, const char *key, size_t key_len)
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

static int	replace_node_kv(t_list *node, const char *kv_pair)
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

static int	append_new_kv(t_list **env, const char *kv_pair)
{
	t_list	*new_node;
	char	*dup;

	dup = ft_strdup(kv_pair);
	if (!dup)
		return (1);
	new_node = ft_lstnew(dup);
	if (!new_node)
	{
		free(dup);
		return (1);
	}
	ft_lstadd_back(env, new_node);
	return (0);
}

char	*env_get(t_list *env, const char *key)
{
	size_t	key_len;
	char	*entry;

	if (!env || !key)
		return (NULL);
	key_len = ft_strlen(key);
	entry = NULL;
	while (env)
	{
		entry = (char *)env->content;
		if (entry && key_matches(entry, key, key_len))
			return (entry + key_len + 1);
		env = env->next;
	}
	return (NULL);
}

int	env_set(t_shell_data *sh, const char *key, const char *value)
{
	size_t	key_len;
	char	*kv_pair;
	t_list	*node;

	if (!sh || !key || !value)
		return (1);
	key_len = ft_strlen(key);
	kv_pair = ft_str3var(key, "=", value);
	if (!kv_pair)
		return (1);
	node = find_key_node(sh->env, key, key_len);
	if (node)
	{
		if (replace_node_kv(node, kv_pair))
			return (free(kv_pair), 1);
		return (free(kv_pair), 0);
	}
	if (append_new_kv(&sh->env, kv_pair))
		return (free(kv_pair), 1);
	return (free(kv_pair), 0);
}

static int	count_env_nodes(t_list *env)
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

static int	fill_env_array(t_list *env, char **array)
{
	int		i;
	char	*dup;

	i = 0;
	while (env)
	{
		dup = ft_strdup((char *)env->content);
		if (!dup)
			return (1);
		array[i] = dup;
		i++;
		env = env->next;
	}
	array[i] = NULL;
	return (0);
}

char	**env_list_to_array(t_list *env)
{
	int		count;
	char	**array;

	count = count_env_nodes(env);
	array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	if (fill_env_array(env, array))
	{
		ft_array_free(array);
		return (NULL);
	}
	return (array);
}
