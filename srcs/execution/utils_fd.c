
#include <unistd.h>

void	close_pair(int p[2])
{
	if (p[0] >= 0)
		close(p[0]);
	if (p[1] >= 0)
		close(p[1]);
}

int	save_stdio(int fd[2])
{
	fd[0] = dup(STDIN_FILENO);
	if (fd[0] < 0)
		return (-1);
	fd[1] = dup(STDOUT_FILENO);
	if (fd[1] < 0)
	{
		close(fd[0]);
		return (-1);
	}
	return (0);
}

int	restore_stdio(int fd[2])
{
	int	r1;
	int	r2;

	r1 = dup2(fd[0], STDIN_FILENO);
	r2 = dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	if (r1 < 0 || r2 < 0)
		return (-1);
	return (0);
}
