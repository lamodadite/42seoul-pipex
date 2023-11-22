#include "pipex.h"

char	*get_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], PATH, 5) == 0)
			return (envp[i]);
		i++;
	}
	return (NULL);
}

//char	*get_cmd(char *path, char *cmd)
//{
	
//}

int main(int ac, char **av, char **envp)
{
	(void) envp;
	char *infile;
	char *outfile;
	int	pipe_fds[2];
	int fd1;
	int fd2;
	int pid;
	int wstatus;
	char **cmd;

	if (ac < 5)
		return (-1);
	infile = av[1];
	outfile = av[4];

	// 파이프를 연다
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe()");
		return (-1);
	}
	// 자식 프로세스를 생성
	pid = fork();
	// 자식 프로세스일때
	if (pid == 0)
	{
		// infile open
		fd1 = open(infile, O_RDONLY | O_WRONLY);
		if (fd1 == -1)
		{
			perror("open()");
			return (-1);
		}
		// 안쓰는 pipe의 fd를 닫아줌
		close(pipe_fds[0]);
		// 표준입력을 fd1로 바꾼다
		// 표준출력을 pipe_fds[1]로 바꾼다
		if (dup2(fd1, STDIN_FILENO) == -1)
		{
			perror("dup2()");
			return (-1);
		}
		if (dup2(pipe_fds[1], STDOUT_FILENO) == -1)
		{
			perror("dup2()");
			return (-1);
		}
		// split으로 나눠서 [0]은 명령어, [1]은 옵션
		cmd = ft_split(av[2], ' ');
		// [0]에 해당하는 명령어와 같은 문자열을 PATH에서 찾고 1번쨰 인자로 넣기
		// 명령어, 옵션, NULL로 이루어진 이차원 배열 2번째 인자로 넣기
		if (execve("/usr/bin/grep", cmd, envp) == -1)
		{
			perror("execve()");
			return (-1);
		}
	}
	// 부모 프로세스는 자식 프로세스가 종료될때까지 기다림
	if (wait(&wstatus) == -1)
	{
		perror("wait()");
		return (-1);
	}
	// 안쓰는 pipe의 fd를 닫아줌
	close(pipe_fds[1]);
	// 표준입력을 pipe_fds[0]로 바꾼다
	if (dup2(pipe_fds[0], STDIN_FILENO) == -1)
	{
		perror("dup2()");
		return (-1);
	}
	// outfile open
	fd2 = open(outfile, O_CREAT);
	if (fd2 == -1)
	{
		perror("open()");
		return (-1);
	}
	// 표준출력을 outfile로 바꿔줌
	if (dup2(fd2, STDOUT_FILENO) == -1)
	{
		perror("dup2()");
		return (-1);
	}
	// [3]에 해당하는 명령어와 옵션을 스플릿
	cmd = ft_split(av[3], ' ');
	if (execve("/usr/bin/wc", cmd, envp) == -1)
	{
		perror("execve()");
		return (-1);
	}
}
