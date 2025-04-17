/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_and_proc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:25:51 by hsennane          #+#    #+#             */
/*   Updated: 2025/04/14 21:07:51 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	redirect_and_duplicate(t_pipex pipex, int input_fd, int output_fd)
{
	int	dup_stdin_result;
	int	dup_stdout_result;

	dup_stdin_result = dup2(input_fd, STDIN_FILENO);
	if (-1 == dup_stdin_result)
	{
		free_pipex_resources(&pipex);
		cleanup_pipex_resources(&pipex);
		exit(EXIT_FAILURE);
	}
	close(input_fd);
	dup_stdout_result = dup2(output_fd, STDOUT_FILENO);
	if (-1 == dup_stdout_result)
	{
		free_pipex_resources(&pipex);
		cleanup_pipex_resources(&pipex);
		exit(EXIT_FAILURE);
	}
	close(output_fd);
}

void	handle_exec_failure(t_pipex *pipex, char *exec_path, char *arg0,
		int error_code)
{
	if (error_code == 126)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(arg0, STDERR_FILENO);
		if (open(exec_path, O_DIRECTORY) != -1)
			ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		else
			ft_putstr_fd(": permission denied\n", STDERR_FILENO);
	}
	else if (error_code == 127)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(arg0, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	cleanup_pipex_resources(pipex);
	exit(error_code);
}

void	run_child_command(t_pipex pipex, int command_index, int input,
		int output)
{
	char	*exec_path;
	char	**args_list;

	exec_path = pipex.commands[command_index].path;
	args_list = pipex.commands[command_index].args;
	redirect_and_duplicate(pipex, input, output);
	free_pipex_resources(&pipex);
	if (!exec_path || !args_list)
	{
		cleanup_pipex_resources(&pipex);
		exit(127);
	}
	if (open(exec_path, O_DIRECTORY) != -1 || (access(exec_path, F_OK) == 0
			&& access(exec_path, X_OK) == -1))
	{
		handle_exec_failure(&pipex, exec_path, args_list[0], 126);
	}
	if (execve(exec_path, args_list, pipex.envp) == -1)
	{
		handle_exec_failure(&pipex, exec_path, args_list[0], 127);
	}
}

void	manage_child_processes(t_pipex pipex, int index)
{
	if (!pipex.commands[index].args)
	{
		free_pipex_resources(&pipex);
		cleanup_pipex_resources(&pipex);
		exit(EXIT_FAILURE);
	}
	if (index == 0)
		run_child_command(pipex, index, pipex.input_fd, pipex.pipes[index][1]);
	else if (index != pipex.arg_count - 1)
	{
		run_child_command(pipex, index, pipex.pipes[index - 1][0],
			pipex.pipes[index][1]);
	}
	else
		run_child_command(pipex, index, pipex.pipes[index - 1][0],
			pipex.output_fd);
}
