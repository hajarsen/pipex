/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsennane <hsennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:56:12 by hsennane          #+#    #+#             */
/*   Updated: 2025/04/14 21:06:49 by hsennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <errno.h>
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_cmd
{
	int		found;
	char	*path;
	char	**args;
}			t_cmd;

typedef struct s_pipex
{
	char	**envp;
	char	**argv;
	char	**paths;
	t_cmd	*commands;
	int		*process_ids;
	int		**pipes;
	int		input_fd;
	int		output_fd;
	int		arg_count;
	int		exit_status;
}			t_pipex;

/* Libft functions */

char		**ft_split(const char *s, char delim);
void		ft_putstr_fd(char *s, int fd);
int			ft_strncmp(char *s1, char *s2, int len);
char		*ft_strdup(const char *s);
char		*ft_strjoin(char *s1, char *s2);
char		*ft_strjoin_three(char *s1, char *s2, char *s3);
int			ft_strlen(const char *s);

/* Pipex functions */
void		report_missing_command(t_pipex *pipex, int i);
int			free_string_array(char **array);
int			free_pipex_resources(t_pipex *pipex);
int			cleanup_pipex_resources(t_pipex *pipex);
int			initialize_pipes(t_pipex *pipex);
int			initialize_process_ids(t_pipex *pipex);
int			initialize_pipes(t_pipex *pipex);
int			initialize_process_ids(t_pipex *pipex);
int			handle_zombie_processes(t_pipex *pipex);
int			run_pipeline_commands(t_pipex *pipex);
int			validate_command(t_pipex *pipex, char *path, int index);
void		get_command_path(t_pipex *pipex, int index);
void		get_environment_path(t_pipex *pipex);
void		setup_input_output(t_pipex *pipex);
int			initialize_pipex_structure(t_pipex *pipex);
int			initialize_pipeline(t_pipex *pipex, int argc, char **argv,
				char **envp);
int			allocate_command_structures(t_pipex *pipex);
void		handle_exec_failure(t_pipex *pipex, char *exec_path, char *arg0,
				int error_code);
void		run_child_command(t_pipex pipex, int command_index, int input,
				int output);
void		manage_child_processes(t_pipex pipex, int index);
#endif /* PIPEX_H */
