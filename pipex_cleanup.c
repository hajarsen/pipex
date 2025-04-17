/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:43:49 by hsennane          #+#    #+#             */
/*   Updated: 2025/04/15 12:38:03 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	free_string_array(char **array)
{
	int	i;

	if (!array)
		return (0);
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (1);
}

int	free_pipex_resources(t_pipex *pipex)
{
	int	i;

	if (!pipex || !pipex->pipes)
		return (0);
	i = 0;
	while (i < pipex->arg_count && pipex->pipes[i])
	{
		if (pipex->pipes[i])
		{
			close(pipex->pipes[i][0]);
			close(pipex->pipes[i][1]);
			free(pipex->pipes[i]);
		}
		i++;
	}
	free(pipex->pipes);
	if (-1 != pipex->input_fd)
		close(pipex->input_fd);
	if (-1 != pipex->output_fd)
		close(pipex->output_fd);
	return (1);
}

int	cleanup_pipex_resources(t_pipex *pipex)
{
	int	i;

	if (!pipex)
		return (0);
	i = 0;
	while (i < pipex->arg_count)
	{
		free_string_array(pipex->commands[i].args);
		if (pipex->commands[i].found)
			free(pipex->commands[i].path);
		i++;
	}
	free(pipex->commands);
	if (pipex->paths)
		free_string_array(pipex->paths);
	if (-1 != pipex->input_fd)
		close(pipex->input_fd);
	if (-1 != pipex->output_fd)
		close(pipex->output_fd);
	if (pipex->process_ids)
		free(pipex->process_ids);
	pipex->process_ids = NULL;
	return (1);
}
