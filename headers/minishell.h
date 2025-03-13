/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 20:38:14 by paude-so          #+#    #+#             */
/*   Updated: 2025/03/13 14:36:11 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "ft_string.h"
# include "ft_util.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

typedef long long		t_size_ll;
typedef struct s_token	t_token;
typedef struct s_cmd	t_cmd;

typedef enum e_cmd_type
{
	CMD,
	PIPE,
	OR,
	AND
}						t_cmd_type;

typedef enum e_redirect_type
{
	IN,
	OUT
}						t_redirect_type;

typedef struct s_terminal
{
	void				*env;
	t_token				*token;
	int					status;
	struct sigaction	sa;
}						t_terminal;

typedef struct s_redirect
{
	t_redirect_type		type;
	char				**args;
	struct s_redirect	*next;
}						t_redirect;

struct					s_cmd
{
	char				**args;
	pid_t				(*execute)(t_cmd * cmd, int in, int out);
	int					in;
	int					out;
	t_redirect			*redirect;
};

struct					s_token
{
	t_cmd_type			type;
	t_cmd				*cmd;
	int					balancing;
	struct s_token		*left;
	struct s_token		*right;
	pid_t				pid;
};

typedef struct s_lexer
{
	char				*input;
	size_t				pos;
	size_t				len;
	char				curr_char;
}						t_lexer;

// lexer
t_lexer					*init_lexer(char *input);
void					advance_lexer(t_lexer *lexer);
void					skip_whitespace(t_lexer *lexer);
char					*read_operator(t_lexer *lexer);
char					*read_redirection(t_lexer *lexer);
char					*read_string(t_lexer *lexer, char quote);
char					*read_word(t_lexer *lexer);
char					*read_parenthesis(t_lexer *lexer);
char					*handle_special_char(t_lexer *lexer);
void					add_token_to_array(t_array *tokens, char *token);
// parse
char					**tokenize(char *input);
t_token					*parse(char *line);
bool					free_token(t_token *token);
void					free_redirect(t_redirect *r);
t_token					*new_token(char *type, t_cmd *cmd);
// env
void					init_env(char **env);
// terminal
t_terminal				*terminal(void);
void					ft_exit(void);
// execution
char					*get_command_path(char *cmd);
void					process_token(t_token *token);
void					loop(void);
// cmd
void					free_cmd(t_cmd *cmd);
t_cmd					*new_cmd(char **args);
// utils
void					print_list(char **args);
int						ft_close(int fd);

#endif
