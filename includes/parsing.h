/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:07:13 by nweber            #+#    #+#             */
/*   Updated: 2025/08/24 19:48:03 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdbool.h>
# include "structs.h"

typedef enum e_token_state
{
	SINGLE_Q,
	DOUBLE_Q,
	GENERAL,
	EXPAND,
}	t_token_state;

typedef enum e_token_type
{
	WORD,
	PIPE,
	HERE_DOC,
	APPEND,
	INFILE,
	OUTFILE,
	AND,
	OR,
	PARENTHESIS,
}	t_token_type;

typedef struct s_token
{
	int				position;
	char			*value;
	t_token_state	state;
	t_token_type	type;
}					t_token;

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
void			set_token_position(t_list *list);
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

#endif