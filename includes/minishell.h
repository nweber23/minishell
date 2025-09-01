/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:00:25 by nweber            #+#    #+#             */
/*   Updated: 2025/09/01 10:00:02 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "parsing.h"
# include "structs.h"
# include "error.h"
# include "execution.h"
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
int		exit_code(int code);
bool	error_message(char *str);
void	balance_message(int balance);

#endif
