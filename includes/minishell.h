/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:00:25 by nweber            #+#    #+#             */
/*   Updated: 2025/08/25 17:15:46 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "parsing.h"
# include "structs.h"
# include "error.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <stdbool.h>
# include <fcntl.h>
# include <sys/wait.h>

/*************************************************/
/*                  ERROR_HANDLING               */
/*************************************************/
void	free_shell(t_shell_data *shell);
void	free_env(t_list *env);
void	free_token(void *content);
void	error_malloc(char *function, t_shell_data *shell);
int		exit_code(int *code);
bool	error_message(char *str);

#endif