/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jongmlee <jongmlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 22:33:44 by jongmlee          #+#    #+#             */
/*   Updated: 2023/12/11 14:52:24 by jongmlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>

# define PATH "PATH="
# define BASIC_PATH "/usr/local/bin:/usr/bin:/bin:\
	/usr/sbin:/sbin:/usr/local/munki:/Library/Apple/usr/bin"
# define CMD_NOT_FOUND_ERR ": command not found\n"
# define HEREDOC "here_doc"

typedef struct s_info
{
	char	**av;
	char	**envp;
	int		*pipe_fds;
	pid_t	last_pid;
	int		infile_fd;
	int		outfile_fd;
	int		idx;
	int		ac;
	int		cur;
	int		is_heredoc;
}	t_info;

// libft
char	**ft_split(char const *s, char c);
size_t	ft_strlen(const char *s);
int		ft_putstr_fd(char *s, int fd);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strdup(const char *s1);
char	*get_next_line(int fd);
void	*ft_calloc(size_t count, size_t size);

// exit
void	free_2d_array(char **arr);
void	perror_exit(char *str, int exit_code);

// parse
char	*get_path(char **envp);
char	*make_cmd_path(char const *path, char const *cmd);
char	*get_valid_path(char **cmds, char *env_path);
int		execute_cmd(t_info *info);

// utils
void	open_pipe(t_info *info);
void	close_pipe(t_info *info);
void	open_file(t_info *info);
void	close_all_pipe(t_info *info);
void	dup2_sub(int first, int second);

// main
int		wait_children(t_info *info);
void	init_info(t_info *info, int ac, char **av, char **envp);
void	child(t_info *info);
void	here_doc(t_info *info);

char	**ms_split(char *cmd);
int	ms_split_input(char *toss, char **cmd, char *oper);
int	ms_split_first(char *toss, char **cmd, char *oper);
int	ms_split_plus(char *toss, int *i, char **cmd);
int	ms_init(char c, char *s);

#endif