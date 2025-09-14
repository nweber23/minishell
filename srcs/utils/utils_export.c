/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:31:13 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/14 13:52:27 by nweber           ###   ########.fr       */
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

static void	print_decl_x_kv(const char *kv)
{
	size_t	klen;

	klen = key_len_until_equal(kv);
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	write(STDOUT_FILENO, kv, klen);
	ft_putstr_fd("=\"", STDOUT_FILENO);
	ft_putendl_fd((char *)(kv + klen + 1), STDOUT_FILENO);
}

static void	print_export_env_vars(t_list *env)
{
	t_list	*n;
	char	*kv;

	n = env;
	while (n)
	{
		kv = (char *)n->content;
		if (kv && ft_strchr(kv, '='))
			print_decl_x_kv(kv);
		n = n->next;
	}
}

static void	print_export_names(t_list *export_list, t_list *env)
{
	t_list	*n;
	char	*name;
	size_t	klen;

	n = export_list;
	while (n)
	{
		name = (char *)n->content;
		klen = ft_strlen(name);
		if (!find_key_node(env, name, klen))
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putendl_fd(name, STDOUT_FILENO);
		}
		n = n->next;
	}
}

int	print_export_list(t_shell_data *sh)
{
	print_export_env_vars(sh->env);
	print_export_names(sh->export_list, sh->env);
	return (0);
}
