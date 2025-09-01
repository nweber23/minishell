/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:23:03 by nweber            #+#    #+#             */
/*   Updated: 2025/09/01 09:27:08 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

// Main shell state containing environment, input, and parsed data
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

// ABT node types for different command operations
typedef enum e_node_type
{
	N_PIPE,
	N_EXEC,
	N_AND,
	N_OR,
}	t_node_type;

// ABT node for logical OR operations with left and right operands
typedef struct s_or_point
{
	t_node_type	type;
	void		*left;
	void		*right;
}				t_or_point;

// ABT node for logical AND operations with left and right operands
typedef struct s_and_point
{
	t_node_type	type;
	void		*left;
	void		*right;
}				t_and_type;

// Token parsing states for quote handling and expansion
typedef enum e_token_state
{
	SINGLE_Q,
	DOUBLE_Q,
	GENERAL,
	EXPAND,
}	t_token_state;

// Token types for shell operators and words
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

// Individual token with position, value, state, and type information
typedef struct s_token
{
	int				position;
	char			*value;
	t_token_state	state;
	t_token_type	type;
}					t_token;

#endif
