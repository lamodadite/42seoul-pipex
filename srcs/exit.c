#include "pipex.h"

void	perror_exit(char *str, int exit_code)
{
	perror(str);
	exit(exit_code);
}