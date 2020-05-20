#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>


#ifndef GRADING

#define TEST_SYMBOL1 "sys_open"
#define TEST_SYMBOL2 "sys_close"
#define TEST_SYMBOL3 "sys_time"
#define TEST_SYMBOL4 "invalid_symbol"

#endif

//structure for mode
struct dumpmode_t {
	unsigned int mode;
};

//file open thread function for sys_open
void* openf(void* data)
{
	int fd;
	fd = open("/dev/tty", O_RDWR);
	close(fd);
	return 0;
}

//file close thread function for sys_close
void* closef(void* data)
{
	int fd;
	fd = open("/dev/tty", O_RDWR);
	close(fd);
	return 0;
}

//time thread function for sys_time
void* timef(void* data)
{
	time_t sec;
	sec = time(NULL);
	printf("seconds since January 1, 1970 = %ld\n", sec);
	return 0;
}


//start of the main function
int main(int argc, char *argv[])
{
	char name[100];  //character array to pass the name of the symbol to syscall
	struct dumpmode_t modestruct;  //dump mode structure
	int ret,usrmode,symtype,rmid,rmcont,inscont,fd;
	pthread_t t1,t2,t3;
	pid_t pidchild;

	printf("Welcome to Dump Stack Testing\n");
loop:
		printf("Enter the dumpstack mode\n");
		scanf("%d", &usrmode);
		printf("Select the symbol name where you want to insdump 1.sys_open 2.sys_close 3.sys_time 4.invalid_symbol\n");
		printf("Enter 1,2,3,4 to select the symbol\n");
		scanf("%d", &symtype);
		modestruct.mode = usrmode;  //take the mode entered by the user in the structure to pass that mode through the syscall
		//do insdump, create thread and do rmdump for the first symbol
		if(symtype == 1)
		{
		
			printf("Inserting Insdump\n");
			snprintf(name, sizeof(char)*100, "%s", TEST_SYMBOL1);
			ret = syscall(359, name, &modestruct);
			if(ret<0)
			{
				printf("Failed insdump\n");
				return -1;
			}
			printf("Dumpstack I'd: %d\n",ret);

			
			pthread_create(&t1, NULL, openf, (void *)NULL);
			pthread_join(t1, NULL);

			rmid = ret;
			printf("Executing Rmdump\n");
			ret = syscall(360, rmid);
			if(ret<0)
			{
			printf("Failed rmdump\n");
			}
			
		}
		//do insdump, create thread and do rmdump for the second symbol
		else if(symtype == 2)
		{

			snprintf(name, sizeof(char)*100, "%s", TEST_SYMBOL2);
			printf("Inserting Insdump\n");
			ret = syscall(359, name, &modestruct);
			if(ret<0)
			{
				printf("Failed insdump\n");
				return -1;
			}
			printf("Dumpstack I'd: %d\n", ret);

			
			pthread_create(&t2, NULL, closef, (void *)NULL);
			pthread_join(t2, NULL);

			rmid = ret;
			printf("Executing Rmdump\n");
			ret = syscall(360, rmid);
			if(ret<0)
			{
			printf("Failed rmdump\n");
			}
			
		}
		//do insdump, create thread and do rmdump for the third symbol
		else if(symtype == 3)
		{
			
			snprintf(name, sizeof(char)*100, "%s", TEST_SYMBOL3);
			printf("Inserting Insdump\n");
			ret = syscall(359, name, &modestruct);
			if(ret<0)
			{
				printf("Failed insdump\n");
				return -1;
			}
			printf("Dumpstack I'd: %d\n", ret);

			
			pthread_create(&t3, NULL, timef, (void *)NULL);
			pthread_join(t3, NULL);

			rmid = ret;
			printf("Executing Rmdump\n");
			ret = syscall(360, rmid);
			if(ret<0)
			{
			printf("Failed rmdump\n");
			}
			
		}

		else if(symtype == 4)
		{
			snprintf(name, sizeof(char)*100, "%s", TEST_SYMBOL4);
			ret = syscall(359, name, &modestruct);
			if(ret<0)
			{
				printf("Failed, symbol not defined\n");
				
			}
		}
		else
		{
			printf("Wrong choice of symbol\n");
		}

		printf("Continue?\n");
		printf("Enter 1 to continue dumpstack testing else enter any other key to abort\n");
		scanf("%d", &inscont);
		if(inscont == 1)
		{
			goto loop;
		}
		
/////////////////////////////////////////////////////////////////////////////

		printf("Checking for rmdump, insertion of wrong dumpstack I'D\n");	
		//do insdump
		printf("Inserting Insdump for Symbol3\n");
		snprintf(name, sizeof(char)*100, "%s", TEST_SYMBOL1);
		ret = syscall(359, name, &modestruct);
		if(ret<0)
		{
			printf("Failed insdump\n");
			return -1;
		}
		printf("Dumpstack I'd: %d\n",ret);
		printf("Executing Rmdump\n");

loop2:
		//do rmdump
		printf("Enter the dumpstack I'd for rmdump\n");
		scanf("%d", &rmid);
		
		ret = syscall(360, rmid);
		if(ret<0)
		{
			printf("Failed, incorrect dumpstack I'd\n");
		}
		printf("Dumpstack I'd %d is removed\n", rmid);
		
		printf("Continue?\n");
		printf("Enter 1 to continue entering another dumpstack I'd else enter any other key to abort\n");
		scanf("%d", &rmcont);
		if(rmcont == 1)
		{
			goto loop2;
		}
	
////////////////////////////////////////////////////////////////////////////

	//enter the  mode and do insdump
	printf("Checking for other mode\n");
	printf("Enter the dumpstack mode\n");
	scanf("%d", &usrmode);
	modestruct.mode = usrmode;
	snprintf(name, sizeof(char)*100, "%s", TEST_SYMBOL1);
	ret = syscall(359, name, &modestruct);
	if(ret<0)
	{
		printf("Failed insdump\n");
		return -1;
	}
	printf("Dumpstack I'd: %d\n",ret);	
	//create the child process
        //test for mode 1
	pidchild = fork();
	if(pidchild <=0)
	{
		printf("Inside child\n");
		fd = open("/dev/tty", O_RDWR);
		close(fd);
	}

	pthread_create(&t1, NULL, openf, (void *)NULL);
	pthread_join(t1, NULL);
	//do rmdump
	rmid = ret;
	ret = syscall(360, rmid);
	if(ret<0)
	{
	printf("Failed rmdump\n");
	}

/////////////////////////////////////////////////////////////////////////////

	printf("Testing for removal of insdump when program is Killed after insdump and insertion of multiple insdump\n");
	printf("Enter the dumpstack mode\n");
	scanf("%d", &usrmode);   //enter the dumpstack mode for multiple insdumps
	modestruct.mode = usrmode;
	snprintf(name, sizeof(char)*100, "%s", TEST_SYMBOL2);
	ret = syscall(359, name, &modestruct);
	if(ret<0)
	{
		printf("Failed insdump\n");
		return -1;
	}
	printf("Dumpstack I'd: %d\n", ret);


	snprintf(name, sizeof(char)*100, "%s", TEST_SYMBOL1);
	ret = syscall(359, name, &modestruct);
	if(ret<0)
	{
		printf("Failed insdump\n");
		return -1;
	}
	printf("Dumpstack I'd: %d\n",ret);
	
	//return without doing rmdump to test the functionality of automatic rmdump, if there are insdumps
	return 0;

}
