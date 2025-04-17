/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:54:27 by hsennane          #+#    #+#             */
/*   Updated: 2025/04/15 12:50:05 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	initialize_pipes(t_pipex *pipex)
{
	int	i;

	pipex->pipes = malloc(sizeof(int *) * pipex->arg_count);
	if (!pipex->pipes)
		return (0);
	i = 0;
	while (i < pipex->arg_count)
	{
		pipex->pipes[i] = malloc(sizeof(int) * 2);
		if (!pipex->pipes[i])
			return (0);
		if (pipe(pipex->pipes[i]) == -1)
			return (0);
		i++;
	}
	return (1);
}

int	initialize_process_ids(t_pipex *pipex)
{
	int	i;

	pipex->process_ids = malloc(sizeof(int) * pipex->arg_count);
	if (!pipex->process_ids)
		return (0);
	i = 0;
	while (i < pipex->arg_count)
	{
		pipex->process_ids[i] = -1;
		i++;
	}
	return (1);
}

int	handle_zombie_processes(t_pipex *pipex)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < pipex->arg_count && pipex->process_ids[i] > 0)
	{
		waitpid(pipex->process_ids[i], &status, 0);
		if (WIFEXITED(status))
			pipex->exit_status = WEXITSTATUS(status);
		i++;
	}
	return (1);
}

int	run_pipeline_commands(t_pipex *pipex)
{
	int		i;
	pid_t	child_pid;

	if (initialize_process_ids(pipex) == 0)
		return (0);
	i = 0;
	while (i < pipex->arg_count)
	{
		child_pid = fork();
		if (-1 == child_pid)
		{
			free_pipex_resources(pipex);
			return (0);
		}
		else if (0 == child_pid)
			manage_child_processes(*pipex, i);
		else
			pipex->process_ids[i] = child_pid;
		i++;
	}
	free_pipex_resources(pipex);
	handle_zombie_processes(pipex);
	return (1);
}
