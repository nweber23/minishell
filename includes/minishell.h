/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:00:25 by nweber            #+#    #+#             */
/*   Updated: 2025/09/12 11:44:38 by nweber           ###   ########.fr       */
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
# include <signal.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>

/*************************************************/
/*                  ERROR_HANDLING               */
/*************************************************/
void	free_shell(t_shell_data *shell);
void	free_env(t_list *env);
void	free_token(void *content);
void	free_logic_tree(void *root);
void	free_binary(void *root);
void	free_pipe(t_pipe *pipe);
void	free_exec(t_exec *exec);
void	error_malloc(char *function, t_shell_data *shell);
int		exit_code(int code);
bool	error_message(char *str);
void	exit_msg(void);
void	balance_message(int balance);

/*************************************************/
/*                     LOOP                      */
/*************************************************/
void	reset_shell(t_shell_data *shell);
int		end_process(int value);
void	input(t_shell_data *shell);
int		minishell_loop(t_shell_data *shell, char **envp);
void	setup_readline_tty_once(void);
char	*read_line_noninteractive(void);
int		is_interactive(void);
void	init_iteration(t_shell_data *shell);

/*************************************************/
/*                    UTILS                      */
/*************************************************/
int		fill_env_array(t_list *env, char **array);
int		count_env_nodes(t_list *env);
int		replace_node_kv(t_list *node, const char *kv_pair);
t_list	*find_key_node(t_list *env, const char *key, size_t key_len);
int		key_matches(const char *entry, const char *key, size_t key_len);

#endif
