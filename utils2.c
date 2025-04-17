/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:54:03 by hsennane          #+#    #+#             */
/*   Updated: 2025/04/14 13:01:14 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	j = 0;
	str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strjoin_three(char *s1, char *s2, char *s3)
{
	char	*res1;
	char	*result;

	if (!s1 || !s2 || !s3)
		return (NULL);
	res1 = ft_strjoin(s1, s2);
	if (!res1)
		return (NULL);
	result = ft_strjoin(res1, s3);
	if (!result)
	{
		free(res1);
		return (NULL);
	}
	free(res1);
	return (result);
}
