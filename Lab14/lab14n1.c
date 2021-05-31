#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

int main(void) {
 
  (void)signal(SIGINT, SIG_IGN);	 
  (void)signal(SIGQUIT, SIG_IGN);	 
 
  while(1);
  return 0;
}
