#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	pid_t pid, ppid;
	int a = 0;

	fork();

	a = a + 1;

	pid = getpid();
	ppid = getppid();

	// ѕри успешном созданнии дочернего процесса, доч. ppid == род. pid
	// pid родительского и дочернего процесса будет различатьс€ на маленькое число, т.к. они создаютс€ "почти одновременно" 
	// ѕри каждом запуске программы id мен€ютс€ 
	// Ќе всегда дочерний процесс может создатьс€ == одна строка выведетьс€  
	printf("My pid = %d, my ppid = %d, result = %d\n", (int)pid, (int)ppid, a);

	return 0;
}
