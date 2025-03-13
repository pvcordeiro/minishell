/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:32:12 by paude-so          #+#    #+#             */
/*   Updated: 2025/03/13 12:26:55 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_token	*new_token(char *type, t_cmd *cmd)
{
    t_token	*t;

    if (!type)
        return (NULL);
    t = ft_calloc(sizeof(t_token));
    if (!t)
        return (NULL);
    t->cmd = cmd;
    if (str().equals("|", type))
    {
        t->type = PIPE;
        t->balancing = 1;
    }
    else if (str().equals("||", type))
    {
        t->type = OR;
        t->balancing = 1;
    }
    else if (str().equals("&&", type))
    {
        t->type = AND;
        t->balancing = 1;
    }
    else
        t->type = CMD;
    t->pid = 0;
    return (t);
}

bool	free_token(t_token *token)
{
    if (!token)
        return (false);
    free_cmd(token->cmd);
    free_token(token->left);
    free_token(token->right);
    free(token);
    return (true);
}
