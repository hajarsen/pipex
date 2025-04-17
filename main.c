/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:20:40 by hsennane          #+#    #+#             */
/*   Updated: 2025/04/15 12:51:48 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	validate_argument_count(int argc)
{
	if (argc != 5)
	{
		ft_putstr_fd("Error: Invalid number of arguments\n", 2);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	setup_pipex(t_pipex *pipex, int argc, char **argv, char **envp)
{
	if (initialize_pipeline(pipex, argc, argv, envp) == 0)
	{
		ft_putstr_fd("Error: Failed to initialize pipeline\n", 2);
		return (cleanup_pipex_resources(pipex), EXIT_FAILURE);
	}
	if (initialize_pipex_structure(pipex) == 0)
	{
		ft_putstr_fd("Error: Failed to initialize pipex structure\n", 2);
		return (cleanup_pipex_resources(pipex), EXIT_FAILURE);
	}
	if (initialize_pipes(pipex) == 0)
	{
		ft_putstr_fd("Error: Failed to initialize pipe array\n", 2);
		return (cleanup_pipex_resources(pipex), EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	execute_and_cleanup(t_pipex *pipex)
{
	if (run_pipeline_commands(pipex) == 0)
	{
		ft_putstr_fd("Error: Failed to execute command pipeline\n", 2);
		return (cleanup_pipex_resources(pipex), pipex->exit_status);
	}
	if (cleanup_pipex_resources(pipex) == 0)
	{
		ft_putstr_fd("Error: Failed to cleanup pipex resources\n", 2);
		return (EXIT_FAILURE);
	}
	return (pipex->exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		status;

	status = validate_argument_count(argc);
	if (status != EXIT_SUCCESS)
		return (status);
	status = setup_pipex(&pipex, argc, argv, envp);
	if (status != EXIT_SUCCESS)
		return (status);
	return (execute_and_cleanup(&pipex));
}
