#include "pipex.h"

void	init_info(t_info *info, int ac, char **av, char **envp)
{
	info->idx = -1;
	info->ac = ac;
	info->av = av;
	info->envp = envp;
	info->cur = 0;
	info->pipe_fds = (int *)malloc((ac - 4) * 2 * sizeof(int));
	if (info->pipe_fds == NULL)
		perror_exit("malloc()", 1);
}

int	wait_children(t_info *info)
{
	int	i;
	int wstatus;
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
		if (info->idx == 0)
			dup2_sub(info->infile_fd, info->pipe_fds[info->cur + 1]);
		else if (info->idx == info->ac - 4)
			dup2_sub(info->pipe_fds[info->cur - 2], info->outfile_fd);
		else
			dup2_sub(info->pipe_fds[info->cur - 2], info->pipe_fds[info->cur + 1]);
		if (execute_cmd(info) == -1)
			perror_exit("execve()", 1);
	}
}

void    here_doc(t_info *info)
{
    int        file;
    char    *line;

    unlink(".heredoc_tmp");
    file = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (file < 0)
        perror_exit("open()", 1);
    while (1)
    {
        write(1, "heredoc> ", 9);
        line = get_next_line(0);
        if (!line)
            exit(1);
        if (!ft_strncmp(info->av[info->idx + 2], line, ft_strlen(info->av[info->idx + 2])) \
            && ft_strlen(info->av[info->idx + 2]) == (ft_strlen(line) - 1))
            break ;
        write(file, line, ft_strlen(line));
        free(line);
    }
    free(line);
    close(file);
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
	close_all_pipe(&info);
	exit_code = wait_children(&info);
	return (exit_code);
}
