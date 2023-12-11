/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jongmlee <jongmlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 22:25:31 by jongmlee          #+#    #+#             */
/*   Updated: 2023/11/27 10:24:14 by jongmlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_info(t_info *info, int ac, char **av, char **envp)
{
	int	i;

	i = 0;
	info->idx = -1;
	info->ac = ac;
	info->av = av;
	info->envp = envp;
	info->cur = 0;
	info->pipe_fds = (int *)malloc((ac - 4) * 2 * sizeof(int));
	if (info->pipe_fds == NULL)
		perror_exit("malloc()", 1);
	while (i < (ac - 4) * 2)
		info->pipe_fds[i++] = -1;
	info->is_heredoc = 0;
	if (ft_strncmp(info->av[1], HEREDOC, ft_strlen(HEREDOC)) == 0)
	{
		here_doc(info);
		info->is_heredoc = 1;
		info->idx = 0;
	}
}

int	wait_children(t_info *info)
{
	int	i;
	int	wstatus;
	int	exit_code;
	int	result;

	i = 0;
	exit_code = 0;
	while (1)
	{
		result = wait(&wstatus);
		if (result == -1)
			break ;
		if (result == info->last_pid)
		{
			if (WIFEXITED(wstatus) != 0)
				exit_code = WEXITSTATUS(wstatus);
			else if (WIFSIGNALED(wstatus) != 0)
				exit_code = WTERMSIG(wstatus);
		}
	}
	free(info->pipe_fds);
	return (exit_code);
}

void	child(t_info *info)
{
	info->cur = info->idx * 2;
	if (info->idx != info->ac - 4)
		open_pipe(info);
	info->last_pid = fork();
	if (info->last_pid == -1)
	{
		wait_children(info);
		perror_exit("fork()", 1);
	}
	if (info->last_pid == 0)
	{
		open_file(info);
		close_pipe(info);
		if (info->idx == 0 || (info->idx == 1 && info->is_heredoc == 1))
			dup2_sub(info->infile_fd, info->pipe_fds[info->cur + 1]);
		else if (info->idx == info->ac - 4)
			dup2_sub(info->pipe_fds[info->cur - 2], info->outfile_fd);
		else
			dup2_sub(info->pipe_fds[info->cur - 2],
				info->pipe_fds[info->cur + 1]);
		if (execute_cmd(info) == -1)
			perror_exit("execve()", 1);
	}
}

void	here_doc(t_info *info)
{
	char	*line;

	unlink(".heredoc_tmp");
	info->infile_fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (info->infile_fd < 0)
		perror_exit("open()", 1);
	while (1)
	{
		write(1, "heredoc> ", 9);
		line = get_next_line(0);
		if (!line)
			exit(1);
		if (!ft_strncmp(info->av[2], line, ft_strlen(info->av[2]))
			&& ft_strlen(info->av[2]) == (ft_strlen(line) - 1))
			break ;
		write(info->infile_fd, line, ft_strlen(line));
		free(line);
	}
	free(line);
	close(info->infile_fd);
}

int	main(int ac, char **av, char **envp)
{
	t_info	info;
	int		exit_code;

	if (ac < 5)
		return (1);
	init_info(&info, ac, av, envp);
	while (++info.idx < ac - 3)
		child(&info);
	close_all_pipe(&info);
	exit_code = wait_children(&info);
	return (exit_code);
}
