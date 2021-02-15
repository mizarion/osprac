#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
	int     fd;
	size_t  size;
	char    string[] = "Hello, world!";
	// �������� ������� ����
	fd = open("myfile", O_RDONLY, 0666);
	if (fd < 0)
	{
		printf("Can\'t open file\n");
		exit(-1);
	}
	// �������� ������� ������ �� �����
	size = read(fd, string, 14);
	if (size != 14)
	{
		printf("Can\'t read all string\n");
		exit(-1);
	}
	// ������� ���� � �������
	printf("%s", string);
	// ������� ������� ����
	if (close(fd) < 0)
	{
		printf("Can\'t close file\n");
		exit(-1);
	}

	return 0;
}
