/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:07:13 by nweber            #+#    #+#             */
/*   Updated: 2025/09/01 09:26:47 by yyudi            ###   ########.fr       */
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
bool			is_wildcard(char **argv);
bool			error_redirect(char *str, int *i, int len);
void			set_token_position(t_list *list);
void			quote_toggling(char c, bool *double_q, bool *single_q);
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

#endif
