/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyudi <yyudi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 11:00:41 by yyudi             #+#    #+#             */
/*   Updated: 2025/08/31 11:00:49 by yyudi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *env_get(t_list *env, const char *key)
{
    size_t klen = ft_strlen(key);
    while (env) {
        char *s = (char *)env->content;
        if (ft_strncmp(s, key, klen) == 0 && s[klen] == '=')
            return s + klen + 1;
        env = env->next;
    }
    return NULL;
}

char **env_list_to_array(t_list *env)
{
    int n = 0;
    t_list *p = env;
    while (p) { n++; p = p->next; }
    char **v = (char **)malloc(sizeof(char *) * (n + 1));
    if (!v) return NULL;
    n = 0;
    while (env) {
        v[n++] = ft_strdup((char *)env->content);
        env = env->next;
    }
    v[n] = NULL;
    return v;
}
