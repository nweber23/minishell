/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:23:03 by nweber            #+#    #+#             */
/*   Updated: 2025/08/24 19:50:24 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct s_shell_data
{
	int		fd;
	int		exit_code;
	void	*root;
	char	*input;
	char	*trimmed;
	char	*command_path;
	char	**env_array;
	t_list	*env;
	t_list	*path;
	t_list	*tokens;
}			t_shell_data;

typedef enum e_node_type
{
	N_PIPE,
	N_EXEC,
	N_AND,
	N_OR,
}	t_node_type;

typedef struct s_or_point
{
	t_node_type	type;
	void		*left;
	void		*right;
}				t_or_point;

typedef struct s_and_point
{
	t_node_type	type;
	void		*left;
	void		*right;
}				t_and_type;

#endif