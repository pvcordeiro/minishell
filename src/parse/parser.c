/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:25:48 by paude-so          #+#    #+#             */
/*   Updated: 2025/03/13 13:05:24 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	is_redirection(char *token)
{
	return (str().equals(token, "<") || str().equals(token, ">")
		|| str().equals(token, "<<") || str().equals(token, ">>"));
}

static t_redirect	*create_redirection(char **tokens, size_t *pos)
{
	t_redirect	*redirect;
	char		**args;

	if (!tokens[*pos] || !tokens[*pos + 1])
		return (NULL);
	redirect = ft_calloc(sizeof(t_redirect));
	if (!redirect)
		return (NULL);
	args = ft_calloc(3 * sizeof(char *));
	if (!args)
		return (free(redirect), NULL);
	args[0] = str().copy(tokens[*pos]);
	args[1] = str().copy(tokens[*pos + 1]);
	redirect->args = args;
	if (str().equals(args[0], "<") || str().equals(args[0], "<<"))
		redirect->type = IN;
	else
		redirect->type = OUT;
	*pos += 2;
	return (redirect);
}

static void	add_redirection(t_cmd *cmd, t_redirect *new_redirect)
{
	t_redirect	*redirect;

	if (!cmd || !new_redirect)
		return ;
	if (!cmd->redirect)
	{
		cmd->redirect = new_redirect;
		return ;
	}
	redirect = cmd->redirect;
	while (redirect->next)
		redirect = redirect->next;
	redirect->next = new_redirect;
}

static t_token	*parse_simple_command(char **tokens, size_t *pos)
{
	t_token		*token;
	t_cmd		*cmd;
	size_t		start;
	t_redirect	*redirect;
	t_array		*cmd_args;
	size_t		i;
	char		**args;

	start = *pos;
	cmd = ft_calloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->in = -2;
	cmd->out = -2;
	while (tokens[*pos] && !str().equals(tokens[*pos], "|")
		&& !str().equals(tokens[*pos], "||") && !str().equals(tokens[*pos],
			"&&"))
	{
		if (is_redirection(tokens[*pos]) && tokens[*pos + 1])
		{
			redirect = create_redirection(tokens, pos);
			add_redirection(cmd, redirect);
		}
		else
			(*pos)++;
	}
	if (*pos > start)
	{
		cmd_args = new_array();
		i = start;
		while (i < *pos)
		{
			if (!is_redirection(tokens[i]) && (i == start
					|| !is_redirection(tokens[i - 1])))
				array(cmd_args)->add(str().copy(tokens[i]));
			i++;
		}
		args = array(cmd_args)->to_str();
		array(cmd_args)->destroy();
		if (!args || !args[0])
			return (NULL);
		free_cmd(cmd);
		token = new_token("CMD", new_cmd(args));
		return (token);
	}
	free_cmd(cmd);
	return (NULL);
}

static t_token	*parse_pipe(char **tokens, size_t *pos)
{
	t_token	*left;
	t_token	*right;
	t_token	*result;

	left = parse_simple_command(tokens, pos);
	if (!left)
		return (NULL);
	while (tokens[*pos] && str().equals(tokens[*pos], "|"))
	{
		(*pos)++;
		right = parse_simple_command(tokens, pos);
		if (!right)
			return (free_token(left), NULL);
		result = new_token("|", NULL);
		if (!result)
			return (free_token(left), free_token(right), NULL);
		result->left = left;
		result->right = right;
		left = result;
	}
	return (left);
}

static t_token	*parse_and_or(char **tokens, size_t *pos)
{
	t_token	*left;
	t_token	*right;
	t_token	*result;
	char	*op_type;

	left = parse_pipe(tokens, pos);
	if (!left)
		return (NULL);
	while (tokens[*pos] && (str().equals(tokens[*pos], "&&")
			|| str().equals(tokens[*pos], "||")))
	{
		op_type = tokens[*pos];
		(*pos)++;
		right = parse_pipe(tokens, pos);
		if (!right)
			return (free_token(left), NULL);
		result = new_token(op_type, NULL);
		if (!result)
			return (free_token(left), free_token(right), NULL);
		result->left = left;
		result->right = right;
		left = result;
	}
	return (left);
}

t_token	*parse(char *line)
{
	char	**tokens;
	size_t	pos;
	t_token	*result;

	if (!line || !*line)
		return (NULL);
	free_token(terminal()->token);
	tokens = tokenize(line);
	if (!tokens)
		return (NULL);
	pos = 0;
	result = parse_and_or(tokens, &pos);
	free_list(tokens);
	terminal()->token = result;
	return (result);
}
