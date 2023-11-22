#include "pipex.h"

char	*get_path(char **envp)
{
	int	i;
	char	*path;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], PATH, 5) == 0)
		{
			path = ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5);
			return (path);
		}
		i++;
	}
	return (NULL);
}

//char	*get_cmd_file(char *path, char *cmd)
//{
	
//}

int main(int ac, char **av, char **envp)
{
	(void) ac;
	(void) av;
	char *path;
	char *cmds;
	int	i;

	path = get_path(envp);
	if (path == NULL)
	{
		ft_putstr_fd("malloc failed or path detect failed", 1);
		return (-1);
	}
	cmds = ft_split(path, ':');

	while (cmds)

	printf("%s\n", path);
	
}