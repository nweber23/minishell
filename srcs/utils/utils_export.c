/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:31:13 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/14 13:33:21 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static size_t	key_len_until_equal(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	return (i);
}

int	export_contains_key(t_list *lst, const char *key)
{
	size_t  klen;
	char	*name;

	klen = ft_strlen(key);
	while (lst)
	{
		name = (char *)lst->content;
		if (name && ft_strlen(name) == klen
			&& ft_strncmp(name, key, klen) == 0)
			return (1);
		lst = lst->next;
	}
	return (0);
}

int	export_add_key(t_shell_data *sh, const char *key)
{
	char	*dup;

	if (!key || export_contains_key(sh->export_list, key))
		return (0);
	dup = ft_strdup(key);
	if (!dup)
		return (1);
	ft_lstadd_back(&sh->export_list, ft_lstnew(dup));
	return (0);
}

int	export_remove_key(t_shell_data *sh, const char *key)
{
	t_list	*prev;
	t_list	*cur;
	size_t	klen;

	prev = NULL;
	cur = sh->export_list;
	klen = ft_strlen(key);
	while (cur)
	{
		if (cur->content && ft_strlen((char *)cur->content) == klen
			&& ft_strncmp((char *)cur->content, key, klen) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				sh->export_list = cur->next;
			free(cur->content);
			free(cur);
			return (0);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}

static void	print_decl_x_kv(const char *kv)
{
	size_t	klen;

	klen = key_len_until_equal(kv);
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	write(STDOUT_FILENO, kv, klen);
	ft_putstr_fd("=\"", STDOUT_FILENO);
	ft_putendl_fd((char *)(kv + klen + 1), STDOUT_FILENO);
}

int	print_export_list(t_shell_data *sh)
{
	t_list	*n;
	char	*kv;
	char	*name;
	size_t	klen;

	n = sh->env;
	while (n)
	{
		kv = (char *)n->content;
		if (kv && ft_strchr(kv, '='))
			print_decl_x_kv(kv);
		n = n->next;
	}
	n = sh->export_list;
	while (n)
	{
		name = (char *)n->content;
		klen = ft_strlen(name);
		if (!find_key_node(sh->env, name, klen))
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putendl_fd(name, STDOUT_FILENO);
		}
		n = n->next;
	}
	return (0);
}
