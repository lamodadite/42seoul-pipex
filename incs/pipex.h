#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h> // exit
# include <stdio.h> // perror
# include <fcntl.h> // file 관련 함수
# include <string.h> // strerror
# include <unistd.h> // pipe
# include <sys/types.h> // fork
# include <sys/wait.h> // wait

# define PATH "PATH="

char	**ft_split(char const *s, char c);
size_t	ft_strlen(const char *s);
int		ft_putstr_fd(char *s, int fd);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);

char	*get_path(char **envp);


#endif