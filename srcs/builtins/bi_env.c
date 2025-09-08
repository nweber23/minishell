/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 10:55:00 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/07 18:03:57 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	bi_env(t_shell_data *shell)
{
	t_list	*node;

	node = shell->env;
	while (node)
	{
		ft_putendl_fd((char *)node->content, STDOUT_FILENO);
		node = node->next;
	}
	return (0);
}
