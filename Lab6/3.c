#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#define BUFFER_SIZE PIPE_BUF

int count_pipe_size(void)
{
	int bytes = 0;
	int pipe_size = 0;
	int fd[2];
	char buf[PIPE_BUF * 2];
	if (pipe(fd) == -1)
	{
		return -1;
	}
	if (fcntl(fd[1], F_SETFL, O_NONBLOCK) == -1)
	{
		return -1;
	}
	while ((bytes = write(fd[1], &buf, sizeof buf)) > 0)
	{
		pipe_size += bytes;
	}
	if (errno != EAGAIN && errno != EWOULDBLOCK)
	{
		pipe_size = -1;
	}
	close(fd[0]);
	close(fd[1]);
	return pipe_size;
}

int main()
{
	int s = count_pipe_size();
	printf("Pipe size = %d\n", s);
	return 0;
}