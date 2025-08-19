/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_iface.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 19:56:04 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/19 09:56:46 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"
#include <stdio.h>

int	is_builtin_name(const char *s)
{
	(void)s;
	return (0);
}

int	run_builtin_parent(t_cmd *c)
{
	(void)c;
	return (0);
}

int	run_builtin_child(t_cmd *c)
{
	(void)c;
	return (0);
}
