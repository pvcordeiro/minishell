/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:25:41 by paude-so          #+#    #+#             */
/*   Updated: 2025/03/13 13:48:25 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

typedef struct s_lexer
{
    char	*input;
    size_t	pos;
    size_t	len;
    char	curr_char;
}	t_lexer;

static t_lexer	*init_lexer(char *input)
{
    t_lexer	*lexer;

    lexer = ft_calloc(sizeof(t_lexer));
    if (!lexer)
        return (NULL);
    lexer->input = input;
    lexer->pos = 0;
    lexer->len = str().size(input);
    lexer->curr_char = input[0];
    return (lexer);
}

static void	advance_lexer(t_lexer *lexer)
{
    if (lexer->pos < lexer->len)
    {
        lexer->pos++;
        lexer->curr_char = lexer->input[lexer->pos];
    }
    else
        lexer->curr_char = '\0';
}

static void	skip_whitespace(t_lexer *lexer)
{
    while (lexer->curr_char && str().is_space(lexer->curr_char))
        advance_lexer(lexer);
}

static char	*read_operator(t_lexer *lexer)
{
    char	*result;
    char	first_char;

    first_char = lexer->curr_char;
    advance_lexer(lexer);
    if ((first_char == '|' && lexer->curr_char == '|') ||
        (first_char == '&' && lexer->curr_char == '&'))
    {
        result = ft_calloc(3);
        result[0] = first_char;
        result[1] = lexer->curr_char;
        advance_lexer(lexer);
        return (result);
    }
    result = ft_calloc(2);
    result[0] = first_char;
    return (result);
}

static char	*read_redirection(t_lexer *lexer)
{
    char	*result;
    char	first_char;

    first_char = lexer->curr_char;
    advance_lexer(lexer);
    if ((first_char == '>' && lexer->curr_char == '>') ||
        (first_char == '<' && lexer->curr_char == '<'))
    {
        result = ft_calloc(3);
        result[0] = first_char;
        result[1] = lexer->curr_char;
        advance_lexer(lexer);
        return (result);
    }
    result = ft_calloc(2);
    result[0] = first_char;
    return (result);
}

static char	*read_string(t_lexer *lexer, char quote)
{
    size_t	start;
    char	*result;

    advance_lexer(lexer);
    start = lexer->pos;
    while (lexer->curr_char && lexer->curr_char != quote)
        advance_lexer(lexer);
    result = str().copy_n(lexer->input + start, lexer->pos - start);
    if (lexer->curr_char == quote)
        advance_lexer(lexer);
    return (result);
}

static char	*read_word(t_lexer *lexer)
{
    size_t	start;

    start = lexer->pos;
    while (lexer->curr_char && !str().is_space(lexer->curr_char) &&
            lexer->curr_char != '|' && lexer->curr_char != '&' &&
            lexer->curr_char != '<' && lexer->curr_char != '>' &&
            lexer->curr_char != '\'' && lexer->curr_char != '\"' &&
            lexer->curr_char != '(' && lexer->curr_char != ')')
        advance_lexer(lexer);
    return (str().copy_n(lexer->input + start, lexer->pos - start));
}

static char	**lexer_tokenize(char *input)
{
    t_lexer	*lexer;
    t_array	*tokens;
    char	*token;
    char    **result;

    lexer = init_lexer(input);
    tokens = new_array();
    if (!lexer || !tokens)
        return (NULL);
    while (lexer->curr_char)
    {
        skip_whitespace(lexer);
        if (!lexer->curr_char)
            break ;
        if (lexer->curr_char == '|' || lexer->curr_char == '&')
            token = read_operator(lexer);
        else if (lexer->curr_char == '<' || lexer->curr_char == '>')
            token = read_redirection(lexer);
        else if (lexer->curr_char == '\'')
            token = read_string(lexer, '\'');
        else if (lexer->curr_char == '\"')
            token = read_string(lexer, '\"');
        else if (lexer->curr_char == '(' || lexer->curr_char == ')')
        {
            token = ft_calloc(2);
            token[0] = lexer->curr_char;
            advance_lexer(lexer);
        }
        else
            token = read_word(lexer);
        if (token && *token)
            array(tokens)->add(token);
        else
            free(token);
    }
    free(lexer);
    result = array(tokens)->to_str();
    array(tokens)->destroy();
    return (result);
}

char	**tokenize(char *input)
{
    if (!input || !*input)
        return (NULL);
    return (lexer_tokenize(input));
}
