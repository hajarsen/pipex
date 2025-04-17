/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:54:01 by hsennane          #+#    #+#             */
/*   Updated: 2025/04/14 20:27:48 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	is_delimiter(char c, char delimiter)
{
	return (c == delimiter || c == ' ' || c == '\t' || c == '\n');
}

static int	count_tokens(char const *str, char delimiter)
{
	int	token_count;
	int	in_token;

	token_count = 0;
	in_token = 0;
	while (*str)
	{
		if (is_delimiter(*str, delimiter))
			in_token = 0;
		else if (!in_token)
		{
			in_token = 1;
			token_count++;
		}
		str++;
	}
	return (token_count);
}

static char	*extract_next_token(char const **str, char delimiter)
{
	char	*token;
	int		length;
	int		i;

	length = 0;
	while (**str && is_delimiter(**str, delimiter))
		(*str)++;
	while ((*str)[length] && !is_delimiter((*str)[length], delimiter))
		length++;
	token = malloc(sizeof(char) * (length + 1));
	if (!token)
		return (NULL);
	i = 0;
	while (i < length)
	{
		token[i] = (*str)[i];
		i++;
	}
	token[i] = '\0';
	*str += length;
	return (token);
}

static void	free_tokens(char **tokens, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

char	**ft_split(char const *str, char delimiter)
{
	char	**result;
	int		token_count;
	int		i;

	if (!str)
		return (NULL);
	token_count = count_tokens(str, delimiter);
	result = malloc(sizeof(char *) * (token_count + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < token_count)
	{
		result[i] = extract_next_token(&str, delimiter);
		if (!result[i])
		{
			free_tokens(result, i);
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
}
