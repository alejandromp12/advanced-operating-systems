#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/wait.h>
#include <sys/user.h>
#include "syscall.h"

#define MAIN_THREAD 0
#define MAX_NUM_SYSCALL 547
#define MAX_LEN_SYSCALL 400
#define MODE_v 0
#define MODE_V 1

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLUE   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

typedef struct
{
	system_call list[MAX_LEN_SYSCALL];
	int size;
} array;

int MODE;
array system_call_list;


void print_system_call_table()
{	
	printf("\n\n------------------------------------------------------\n");
	printf(BLUE "%16s%s%9s\n"RESET, " ", "Table of System Calls", " ");
	printf("------------------------------------------------------\n");
	printf("|" CYN "%10s" RESET "|" CYN "%20s" RESET "|" CYN "%20s"RESET "|\n", "Code", "Name", "Number of calls");
	
	for(int i = 0; i < system_call_list.size; i ++)
	{	
		system_call tmp = system_call_list.list[i];
		printf("------------------------------------------------------\n");
		if( i % 2)
		{
			printf("|" YEL "%10d" RESET "|" YEL "%20s" RESET "|" YEL "%20d"RESET "|\n", tmp.code, tmp.syscall, tmp.counter);
		}
		else
		{
			printf("|" GRN "%10d" RESET "|" GRN "%20s" RESET "|" GRN "%20d"RESET "|\n", tmp.code, tmp.syscall, tmp.counter);
		}
		
	}
	printf("------------------------------------------------------\n");
}



system_call search_system_call(int code) {

	int i;
    for (i = 0; system_call_table[i].code <= MAX_NUM_SYSCALL; i++) 
	{
        if (system_call_table[i].code == code) 
		{
    		return system_call_table[i];
        }
    }
	return system_call_table[i];
}

int search_system_call_in_array(int code)
{
	for(int i = 0; i < system_call_list.size; i++)
	{
		if(system_call_list.list[i].code == code)
		{
			return i;
		}
	}
	return -1;
}


system_call add_syscall(system_call sc)
{
	int index = search_system_call_in_array(sc.code);
	if(index < 0){
		system_call_list.list[system_call_list.size] = sc;
		system_call_list.list[system_call_list.size].counter += 1;
		index = system_call_list.size;

		system_call_list.size += 1;
		
	}
	else{
		system_call_list.list[index].counter += 1;
	}

	return system_call_list.list[index];
}



void get_syscall(pid_t child)
{
	struct user_regs_struct regs;
    int syscall;

	ptrace(PTRACE_GETREGS, child, 0, &regs); 
	syscall = regs.orig_rax; 
	system_call tmp = search_system_call(syscall);
	tmp = add_syscall(tmp);

	printf(CYN "Code:" YEL" %d" CYN" - Syscall:" YEL" %s()" CYN" - Number of calls: " YEL"%d\n"RESET, tmp.code, tmp.syscall, tmp.counter);
	
	
}


int run_program(int argc, char *argv[])
{
	ptrace(PTRACE_TRACEME);

    return execvp(argv[0], argv);
}

int trace_program(pid_t child)
{
	
	int status;
	system_call_list.size = 0;


	waitpid(child, &status, 0);
	ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);

	while(1) {

		ptrace(PTRACE_SYSCALL, child, NULL, NULL);
		waitpid(child, &status, 0);
		if(WIFEXITED((status))) break;
		
		if(MODE == MODE_V)
		{
			printf("Press Enter to Continue:");  
			getchar();
			
		}
		get_syscall(child);

		ptrace(PTRACE_SYSCALL, child, NULL, NULL);
		waitpid(child, &status, 0);
		if(WIFEXITED((status))) break;
	
	}

	print_system_call_table();

	return 0;
}



int tracing_sys_call(int argc, char *argv[])
{
	pid_t child = fork();

	if(child == 0)
	{
		char *argv2 [argc + 2];
		memcpy(argv2, argv  + 2, argc * sizeof(char*));
		argv2[argc] = NULL;

		return run_program(argc, argv2);
	}
	else
	{
		return trace_program(child);
	}
}


int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf("Error parameters - format:\n\n %s  -<v|V> <Prog> <args Prog>\n\n", argv[0]);
		exit(1);
	}
	if(strcmp(argv[1],"-v") != 0 && strcmp(argv[1], "-V") != 0)
	{
		printf("Error parameters - format:\n\n %s  -<v|V> <Prog> <args Prog>\n\n", argv[0]);
		exit(1);

	}

	MODE = strcmp(argv[1],"-v") == 0 ? MODE_v : MODE_V;

	tracing_sys_call(argc - 2, argv);

	
	return 0;
}
