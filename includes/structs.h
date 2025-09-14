/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:23:03 by nweber            #+#    #+#             */
/*   Updated: 2025/09/14 13:49:07 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "stdio.h"

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
	char	*cwd;
	t_list	*env;
	t_list	*path;
	t_list	*tokens;
	FILE	*rl_tty;
	t_list	*export_list;
}			t_shell_data;

// ABT node types for different command operations
typedef enum e_node_type
{
	ND_EXEC,
	ND_PIPE,
	ND_AND,
	ND_OR,
	ND_GROUP
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
}				t_and_point;

// ABT node for pipe operations with left and right operands
typedef struct s_pipe
{
	t_node_type	type;
	void		*left;
	void		*right;
}				t_pipe;

// ABT node for command execution with command, arguments, and redirections
typedef struct s_exec
{
	t_node_type	type;
	char		*command;
	char		**argv;
	t_list		*infile;
	t_list		*outfile;
}				t_exec;

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

// Input redirection kind: INF = file, HERE = heredoc
typedef enum e_inf
{
	INF,
	HERE,
}	t_inf;

// Input redirection struct: type, heredoc delimiter (eof), or filename (name)
typedef struct s_infile
{
	t_inf	type;
	char	*eof;
	char	*name;
}			t_infile;

// Output redirection kind: APP = overwrite, ADD = append
typedef enum e_outf
{
	APP,
	ADD,
}	t_outf;

// Output redirection struct: type and target filename
typedef struct s_outfile
{
	t_outf	type;
	char	*name;
}			t_outfile;

typedef struct s_env
{
	char	*value;
	char	*content;
	bool	exported;
	bool	printed;
}			t_env;

#endif
