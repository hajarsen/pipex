/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_command_parser.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:03:11 by hsennane          #+#    #+#             */
/*   Updated: 2025/04/15 12:44:27 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	validate_command(t_pipex *pipex, char *path, int index)
{
	if (!path)
		return (0);
	if (access(path, F_OK) == 0)
	{
		pipex->commands[index].path = ft_strdup(path);
		if (!pipex->commands[index].path)
			return (0);
		pipex->commands[index].found = 1;
		free(path);
		return (1);
	}
	if (access(path, F_OK) == 0 && ft_strncmp(path, "/", 1) == 0)
	{
		pipex->commands[index].path = ft_strdup(path);
		if (!pipex->commands[index].path)
			return (0);
		pipex->commands[index].found = 1;
		free(path);
		return (1);
	}
	free(path);
	return (0);
}

void	get_command_path(t_pipex *pipex, int index)
{
	int		path_index;
	char	*full_path;

	if (pipex->commands[index].args[0] && validate_command(pipex,
			ft_strdup(pipex->commands[index].args[0]), index))
		return ;
	if (!pipex->paths)
	{
		report_missing_command(pipex, index);
		return ;
	}
	path_index = 0;
	while (pipex->paths[path_index])
	{
		full_path = ft_strjoin_three(pipex->paths[path_index], "/",
				pipex->commands[index].args[0]);
		if (validate_command(pipex, full_path, index))
			break ;
		path_index++;
		if (!pipex->paths[path_index])
			report_missing_command(pipex, index);
	}
}

void	get_environment_path(t_pipex *pipex)
{
	int	index;

	index = 0;
	if (!pipex->envp)
		return ;
	while (pipex->envp[index] && ft_strncmp(pipex->envp[index], "PATH", 4))
		index++;
	if (pipex->envp[index] && ft_strncmp(pipex->envp[index], "PATH", 4) == 0)
		pipex->paths = ft_split(pipex->envp[index] + 5, ':');
}

void	setup_input_output(t_pipex *pipex)
{
	pipex->input_fd = open(pipex->argv[1], O_RDONLY, 0644);
	if (-1 == pipex->input_fd)
		perror(pipex->argv[1]);
	pipex->output_fd = open(pipex->argv[pipex->arg_count + 2],
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (-1 == pipex->output_fd)
		perror(pipex->argv[pipex->arg_count + 2]);
}

int	initialize_pipex_structure(t_pipex *pipex)
{
	int	cmd_index;

	get_environment_path(pipex);
	setup_input_output(pipex);
	cmd_index = 0;
	while (cmd_index < pipex->arg_count)
	{
		pipex->commands[cmd_index].found = 0;
		pipex->commands[cmd_index].args = ft_split(pipex->argv[cmd_index + 2],
				' ');
		if (!pipex->commands[cmd_index].args)
			return (0);
		if (!(cmd_index == 0 && pipex->input_fd == -1)
			&& !(cmd_index == pipex->arg_count - 1 && pipex->output_fd == -1))
			get_command_path(pipex, cmd_index);
		cmd_index++;
	}
	return (1);
}
