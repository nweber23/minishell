/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:23:03 by nweber            #+#    #+#             */
/*   Updated: 2025/08/19 11:28:29 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

#include "minishell.h"

typedef struct s_command_list
{
	t_command_node			*head;
	t_command_node			*tail;
	size_t					size;
}				t_command_list;

typedef struct s_file_list
{
	t_file_node				*head;
	t_file_node				*tail;
	size_t					size;
}							t_file_list;

typedef struct s_file_node
{
	char					*file;
	int						redirect_type;
	struct s_file_node		*next;
}							t_file_node;

typedef struct s_command_node
{
	int						command_type;
	char					**commands;
	t_file_node				*files;
	struct s_command_node	*next;
}							t_command_node;

#endif