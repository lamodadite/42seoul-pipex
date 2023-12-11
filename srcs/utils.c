/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jongmlee <jongmlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 22:28:19 by jongmlee          #+#    #+#             */
/*   Updated: 2023/12/11 16:37:03 by jongmlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	open_pipe(t_info *info)
{
	if (pipe(info->pipe_fds[info->cur]) == -1)
		perror_exit("pipe()", 1);
}

void	close_pipe(t_info *info)
{
	int	i;

	i = -1;
	if (info->idx == 0 || (info->idx == 1 && info->is_heredoc == 1))
		close(info->pipe_fds[info->cur][0]);
	else if (info->idx == info->ac - 4)
		close(info->pipe_fds[!info->cur][1]);
	else
	{
		close(info->pipe_fds[!info->cur][1]);
		close(info->pipe_fds[info->cur][0]);
	}
}

void	open_file(t_info *info)
{
	if (info->idx == 0)
	{
		info->infile_fd = open(info->av[1], O_RDONLY);
		if (info->infile_fd == -1)
			perror_exit(ft_strjoin("pipex: ", info->av[1]), 1);
	}
	else if (info->idx == 1 && info->is_heredoc == 1)
	{
		info->infile_fd = open(".heredoc_tmp", O_RDONLY);
		if (info->infile_fd == -1)
			perror_exit(ft_strjoin(".heredoc_tmp: ", info->av[1]), 1);
	}
	else if (info->idx == info->ac - 4)
	{
		if (info->is_heredoc == 1)
			info->outfile_fd = open(info->av[info->ac - 1],
					O_CREAT | O_WRONLY | O_APPEND, 0644);
		else
			info->outfile_fd = open(info->av[info->ac - 1],
					O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (info->outfile_fd == -1)
			perror_exit(ft_strjoin("pipex: ", info->av[info->ac - 1]), 1);
	}
}

void	close_all_pipe(t_info *info)
{
	close(info->pipe_fds[0][0]);
	close(info->pipe_fds[0][1]);
	close(info->pipe_fds[1][0]);
	close(info->pipe_fds[1][1]);
}

void	dup2_sub(int first, int second)
{
	(void) second;
	if (dup2(first, STDIN_FILENO) == -1)
		perror_exit("dup2()", 1);
	if (dup2(second, STDOUT_FILENO) == -1)
		perror_exit("dup2()", 1);
}
