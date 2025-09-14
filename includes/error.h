/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 16:00:13 by nweber            #+#    #+#             */
/*   Updated: 2025/09/12 10:29:42 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# define PIPE_MSG "minishell: syntax error: near unexpected token `|'"
# define QUOTE_MSG "minishell: syntax error: we dont handle dquote"
# define SYNTAX_MSG "minishell: syntax error: unexpected end of file"
# define SYNTAX_ERROR "minishell: syntax error: near unexpected token"
# define REDIRECT_MSG "minishell: syntax error: near unexpected token `newline'"
# define OPEN_MSG "minishell: syntax error: \
unexpected end of file, expected `)'"
# define CLOSE_MSG "minishell: syntax error: near unexpected token `)'"

#endif
