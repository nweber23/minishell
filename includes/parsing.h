/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:07:13 by nweber            #+#    #+#             */
/*   Updated: 2025/09/07 17:57:16 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdbool.h>
# include "structs.h"

/*************************************************/
/*                    Utils                      */
/*************************************************/
int				remove_spaces(char *str, int i);
int				remove_quotes(char *str, int i);
int				check_quotes(t_shell_data *shell, \
	char **value, char *str, int i);
int				join_quotes(t_shell_data *shell, char **value, \
	char *str, int i);
int				join_no_quotes(t_shell_data *shell, \
	char **value, char *str, int i);
int				simple(t_shell_data *shell, t_token *token, char *str, int i);
int				heredoc(t_shell_data *shell, t_token *token, char *str, int i);
int				append(t_shell_data *shell, t_token *token, char *str, int i);
int				is_redirect(char *str);
bool			is_quote(char c);
bool			is_space(char c);
bool			is_meta(char c);
bool			is_expandable(char	*token);
bool			is_parenthesis(t_list *node);
bool			is_wildcard(char **argv);
bool			is_valid_wildcard(char *str);
bool			token_check(t_list *node);
bool			error_redirect(char *str, int *i, int len);
void			set_token_position(t_list *list);
void			clear_sublist(t_list *new);
void			quote_toggling(char c, bool *double_q, bool *single_q);
t_list			*remove_parenthesis(t_list *temp);
t_list			*remove_direct(t_list *temp);
t_list			*remove_goto_next(t_list *temp);
t_list			*get_name(t_list *tokens);
t_list			*get_infiles(t_shell_data *shell, \
	t_list *tokens, t_list **infile);
t_list			*get_outfiles(t_shell_data *shell, \
	t_list *tokens, t_list **outfile);
t_list			*new_token(t_list *temp);
t_list			*create_new_sublist(t_list *tokens);
t_list			*word_check(t_list **tokens, char **argv, int *i);
t_list			*arg_check(t_list *tokens, int *count);
t_token_type	get_type(char *str);

/*************************************************/
/*                  TOKENIZER                    */
/*************************************************/
void			create_token(t_shell_data *shell, char *str);
int				or_handling(t_shell_data *shell, char *str, int i);
int				and_handling(t_shell_data *shell, char *str, int i);
int				pipe_handling(t_shell_data *shell, char *str, int i);
int				redirect_handling(t_shell_data *shell, char *str, int i);
int				words_handling(t_shell_data *shell, char *str, int i);
int				parenthesis_handling(t_shell_data *shell, char *str, int i);

/*************************************************/
/*                    PARSING                    */
/*************************************************/
bool			validate_input(t_shell_data *shell);
bool			pipe_check(char *str);
bool			quote_check(char *str);
bool			quote_position_check(char *str);
bool			redirect_check(char *str);
bool			parenthesis_check(char *str);
void			lexer(t_shell_data *shell, char *input);

/*************************************************/
/*                   LOGIC_TREE                  */
/*************************************************/
void			*build_subtree(t_shell_data *shell, t_list *tokens);
void			*build_tree(t_shell_data *shell, t_list *tokens);
void			*create_and(t_shell_data *shell, void *left, void *right);
void			*create_or(t_shell_data *shell, void *left, void *right);
void			*create_pipe(t_shell_data *shell, void *left, void *right);
void			*create_parenthesis(t_shell_data *shell, \
	void *left_node, t_list *tokens);
void			*insert_nodes(t_shell_data *shell, \
	void *left_node, t_list *tokens);
void			*get_and_node(t_shell_data *shell, \
	void *left_node, t_list *tokens);
void			*get_or_node(t_shell_data *shell, \
	void *left_node, t_list *tokens);
void			*get_pipe_node(t_shell_data *shell, \
	void *left_node, t_list *tokens);

/*************************************************/
/*                  BINARY_TREE                  */
/*************************************************/
void			*insert_node(t_shell_data *shell, void *root, t_list *temp);
void			*build_binary_tree(t_shell_data *shell, t_list	*tokens);
void			*create_execution(t_shell_data *shell, t_list *tokens);
char			**get_argv(t_shell_data *shell, t_list *tokens);
int				count_args(t_list *tokens);

/*************************************************/
/*                    SIGNALS                    */
/*************************************************/
void			reset_child_signals(void);
void			interavtive_signals(void);
void			init_signals(void);
void			ignore_pipe(int sig);
void			handle_sigint(int sig);
void			trap_heredoc(void);
void			break_heredoc(int sig);
t_shell_data	*global_shell(t_shell_data *shell, int check);

#endif
