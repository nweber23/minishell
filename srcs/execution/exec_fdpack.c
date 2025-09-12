/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fdpack.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:37:15 by yyudi             #+#    #+#             */
/*   Updated: 2025/09/11 19:15:39 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	fdpack_init(t_fdpack *p)
{
	p->in = -1;
	p->out = -1;
	p->save_in = -1;
	p->save_out = -1;
}

void	fd_apply_inout(t_fdpack *p)
{
	if (p->in != -1)
	{
		p->save_in = dup(STDIN_FILENO);
		dup2(p->in, STDIN_FILENO);
		close(p->in);
	}
	if (p->out != -1)
	{
		p->save_out = dup(STDOUT_FILENO);
		dup2(p->out, STDOUT_FILENO);
		close(p->out);
	}
}

void	fd_restore(t_fdpack *p)
{
	if (p->save_in != -1)
	{
		dup2(p->save_in, STDIN_FILENO);
		close(p->save_in);
	}
	if (p->save_out != -1)
	{
		dup2(p->save_out, STDOUT_FILENO);
		close(p->save_out);
	}
}
