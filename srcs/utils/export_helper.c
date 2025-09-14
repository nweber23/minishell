/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:49:48 by nweber            #+#    #+#             */
/*   Updated: 2025/09/14 13:49:59 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_contains_key(t_list *lst, const char *key)
{
	size_t	klen;
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
