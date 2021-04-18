#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/wait.h>

#define MAIN_THREAD 0


int run_program(int argc, char *argv[])
{
	ptrace(PTRACE_TRACEME);
	kill(getpid(), SIGSTOP);
    return execvp(argv[0], argv);
}

int trace_program(pid_t child)
{

}

int tracing_sys_call(int argc, char *argv[])
{
	

	pid_t child = fork();

	if(child == MAIN_THREAD)
	{
		char *argv2 [argc + 1];
		memcpy(argv2, argv  + 1, argc * sizeof(char*));
		argv2[argc] = NULL;

		return run_program(argc, argv2);
	}
	else
	{
		return 0;
	
	}
}






































int main(int argc, char *argv[])
{

	if(argc < 2)
	{
		printf("Missing parameters - format:\n\n %s  -<v|V> <Prog> <args Prog>\n\n", argv[0]);
		exit(1);
	}

	printf("%d\n", tracing_sys_call(argc - 1, argv));

	return 0;
}
