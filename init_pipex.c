/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:08:44 by hsennane          #+#    #+#             */
/*   Updated: 2025/04/15 12:30:02 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	allocate_command_structures(t_pipex *pipex)
{
	int	i;

	pipex->commands = malloc(sizeof(t_cmd) * pipex->arg_count);
	if (!pipex->commands)
		return (0);
	i = 0;
	while (i < pipex->arg_count)
	{
		pipex->commands[i].args = NULL;
		pipex->commands[i].path = NULL;
		pipex->commands[i].found = 0;
		i++;
	}
	return (1);
}

int	initialize_pipeline(t_pipex *pipex, int argc, char **argv, char **envp)
{
	pipex->argv = argv;
	pipex->envp = envp;
	pipex->paths = NULL;
	pipex->input_fd = -1;
	pipex->output_fd = -1;
	pipex->commands = NULL;
	pipex->process_ids = NULL;
	pipex->exit_status = EXIT_SUCCESS;
	pipex->pipes = NULL;
	pipex->arg_count = argc - 3;
	if (allocate_command_structures(pipex) == 0)
		return (0);
	return (1);
}
