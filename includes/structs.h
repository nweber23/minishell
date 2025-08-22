/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:23:03 by nweber            #+#    #+#             */
/*   Updated: 2025/08/22 14:50:17 by nweber           ###   ########.fr       */
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

#endif