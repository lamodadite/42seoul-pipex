#include "pipex.h"

void	dup2_sub(int first, int second)
{
	if (dup2(first, STDIN_FILENO) == -1)
		perror_exit("dup2()", 1);
	if (dup2(second, STDOUT_FILENO) == -1)
		perror_exit("dup2()", 1);
}

void	open_pipe(t_info *info)
{
	if (info->idx != 0)
	{
		close(info->pipe_fds[!info->cur][1]);
		if (info->idx != 1)
			close(info->pipe_fds[info->cur][0]);
	}
	if (pipe(info->pipe_fds[info->cur]) == -1)
		perror_exit("pipe()", 1);
}

void	close_pipe(t_info *info)
{
	if (info->idx == 0)
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
	else if (info->idx == info->ac - 4)
	{
		info->outfile_fd = open(info->av[info->ac - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (info->outfile_fd == -1)
			perror_exit(ft_strjoin("pipex: ", info->av[info->ac - 1]), 1);
	}
}

void	child(t_info *info)
{
	info->cur = info->idx % 2;
	if (info->idx != info->ac - 4)
		open_pipe(info);
	info->pids[info->idx] = fork();
	if (info->pids[info->idx] == -1)
	{
		wait_children(info);
		perror_exit("fork()", 1);
	}
	if (info->pids[info->idx] == 0)
	{
		open_file(info);
		close_pipe(info);
		if (info->idx == 0)
			dup2_sub(info->infile_fd, info->pipe_fds[info->cur][1]);
		else if (info->idx == info->ac - 4)
			dup2_sub(info->pipe_fds[!info->cur][0], info->outfile_fd);
		else
			dup2_sub(info->pipe_fds[!info->cur][0], info->pipe_fds[info->cur][1]);
		if (execute_cmd(info) == -1)
			perror_exit("execve()", 1);
	}
}

void	init_info(t_info *info, int ac, char **av, char **envp)
{
	info->pipe_fds[0][0] = 0;
	info->pipe_fds[0][1] = 0;
	info->pipe_fds[1][0] = 0;
	info->pipe_fds[1][1] = 0;
	info->idx = -1;
	info->ac = ac;
	info->av = av;
	info->envp = envp;
	info->cur = 0;
	info->pids = (int *)malloc((ac - 3) * sizeof(int));
	if (info->pids == NULL)
		perror_exit("malloc()", 1);
}

int	wait_children(t_info *info)
{
	int	i;
	int wstatus;
	int exit_code;

	i = -1;
	exit_code = 0;
	//while (++i < info->idx)
	//{
	//	waitpid(info->pids[i], &wstatus, 0);
	//	if ((wstatus & 0xFF) != 0)
    //        exit_code = (wstatus >> 8) & 0xFF;
	//}
	wait(&wstatus);
	close(info->pipe_fds[!info->cur][1]);
	if (info->idx != 2)
	{
		close(info->pipe_fds[info->cur][0]);
		close(info->pipe_fds[!info->cur][0]);
	}
	return (exit_code);
}

int main(int ac, char **av, char **envp)
{
	t_info	info;
	int		exit_code;

	if (ac < 5)
		return (1);
	init_info(&info, ac, av, envp);
	while (++info.idx < ac - 3)
		child(&info);
	exit_code = wait_children(&info);
	return (exit_code);
}
