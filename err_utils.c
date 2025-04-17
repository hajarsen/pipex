/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:05:12 by hsennane          #+#    #+#             */
/*   Updated: 2025/04/14 13:06:35 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	report_missing_command(t_pipex *pipex, int i)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	ft_putstr_fd(pipex->commands[i].args[0], STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	pipex->commands[i].found = 0;
}
