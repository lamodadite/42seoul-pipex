#include "pipex.h"

void	dup2_sub(int first, int second)
{
	if (dup2(first, STDIN_FILENO) == -1)
		perror_exit("dup2()", 1);
	if (dup2(second, STDOUT_FILENO) == -1)
		perror_exit("dup2()", 1);
}

void	child(t_info *info)
{
	int	pid;

	if (pipe(info->pipe_fds) == -1)
		perror_exit("pipe()", 1);
	pid = fork();
	if (pid == 0)
	{
		if (info->idx == 0)
			dup2_sub(info->infile_fd, info->pipe_fds[1]);
		else if (info->idx == info->ac - 1)
			dup2_sub(info->pipe_fds[0], info->outfile_fd);
		else
			dup2_sub(info->pipe_fds[0], info->pipe_fds[1]); // TODO: 확인 필요
	}
}

void	init_info(t_info *info, int ac, char **av, char **envp)
{
	info->pipe_fds[0] = 0;
	info->pipe_fds[1] = 0;
	info->infile_fd = open(av[1], O_RDONLY);
	if (info->infile_fd == -1)
		perror_exit(ft_strjoin("pipex: ", av[1]), 1);
	info->outfile_fd = open(av[ac- 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (info->outfile_fd == -1)
		perror_exit(ft_strjoin("pipex: ", av[ac - 1]), 1);
	info->idx = 0;
	info->ac = ac;
	info->av = av;
	info->envp = envp;
}

int main(int ac, char **av, char **envp)
{
	t_info	info;
	char *infile;
	char *outfile;
	int	pipe_fds[2];
	int fd1;
	int fd2;
	int pid;
	int wstatus;

	if (ac < 5)
		return (1);
	init_info(&info, ac, av, envp);
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe()");
		return (1);
	}
	fd1 = open(infile, O_RDONLY);
	if (fd1 == -1)
	{
		perror(ft_strjoin("pipex: ", av[1]));
		return (1);
	}
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fds[0]);
		if (dup2(fd1, STDIN_FILENO) == -1)
		{
			perror("dup2()1");
			return (1);
		}
		if (dup2(pipe_fds[1], STDOUT_FILENO) == -1)
		{
			perror("dup2()2");
			return (1);
		}
		if (execute_cmd(ft_split(av[2], ' '), envp) == -1)
		{
			perror("execve()");
			return (1);
		}
	}
	if (wait(&wstatus) == -1)
	{
		perror("wait()");
		return (1);
	}
	close(pipe_fds[1]);
	if (dup2(pipe_fds[0], STDIN_FILENO) == -1)
	{
		perror("dup2()3");
		return (1);
	}
	fd2 = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd2 == -1)
	{
		perror("open()");
		return (1);
	}
	if (dup2(fd2, STDOUT_FILENO) == -1)
	{
		perror("dup2()4");
		return (1);
	}
	if (execute_cmd(ft_split(av[3], ' '), envp) == -1)
	{
		perror("execve()");
		return (1);
	}
}
