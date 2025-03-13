/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 12:36:36 by paude-so          #+#    #+#             */
/*   Updated: 2025/03/13 19:11:15 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	check_flag(char *str)
{
	if (!str || *str != '-')
		return (false);
	str++;
	while (*str == 'n')
		str++;
	return ((*str == '\0'));
}

pid_t	execute_echo(t_cmd *cmd, int in, int out)
{
	bool	flag;
	size_t	i;
	char	*value;
	char	*content;
	size_t	j;
	int		k;

	if (str().equals(cmd->args[1], "$?"))
	{
		(str().fputnbr)(out, terminal()->status);
		(str().fputstr)(out, "\n");
		ft_close(out);
		ft_close(in);
		return (0);
	}
	flag = check_flag(cmd->args[1]);
	i = flag;
	while (cmd->args[++i])
	{
		if (str().starts_with(cmd->args[i], "sq:"))
			(str().fputstr)(out, cmd->args[i] + 3);
		else if (str().starts_with(cmd->args[i], "dq:"))
		{
			content = cmd->args[i] + 3;
			j = 0;
			while (content[j])
			{
				if (content[j] == '$' && content[j + 1])
				{
					j++;
					char	var_name[256] = {0};
					k = 0;
					while (content[j] && ((str().isalnum)(content[j])
						|| content[j] == '_'))
						var_name[k++] = content[j++];
					value = hashmap(terminal()->env)->get_key(var_name);
					if (value)
						(str().fputstr)(out, value);
				}
				else
				{
					char	c[2] = {content[j], '\0'};
					(str().fputstr)(out, c);
					j++;
				}
			}
		}
		else if (cmd->args[i][0] == '$' && cmd->args[i][1])
		{
			value = hashmap(terminal()->env)->get_key(cmd->args[i] + 1);
			if (value)
				(str().fputstr)(out, value);
		}
		else
			(str().fputstr)(out, cmd->args[i]);
		if (cmd->args[i + 1])
			(str().fputstr)(out, " ");
	}
	if (!flag)
		(str().fputstr)(out, "\n");
	ft_close(out);
	ft_close(in);
	return (0);
}
