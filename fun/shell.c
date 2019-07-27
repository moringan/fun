//example.c

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

/* waitdemo1.c - shows how parent pauses until child finishes
 */

#define	DELAY	2
#define MAX_ARGSLEN 100
#define MAX_ARGLEN 25
#define MAX_ARGS 100

void printargs(char *list[]) {
	for(; *list != NULL; list++)
		printf("%s ", *list);
	printf("\n");
}

int count_spaces(char *string) {
	int count = 0;
	for(; *string != '\0'; string++) 
		if(*string == ' ')
			count++;
	return count;
}

void main()
{
	int  newpid;
	void child_code(), parent_code();
	char *arglist[MAX_ARGS];
	char string1[MAX_ARGSLEN];  
	char string2[MAX_ARGLEN];
	char *buf;
/*
	printf("before: mypid is %d\n", getpid());

	if ( (newpid = fork()) == -1 )
		perror("fork");
	else if ( newpid == 0 )
		child_code(DELAY);
	else
		parent_code(newpid);
*/
	while(1) {
		//read input from the user
		if (fgets(string1, MAX_ARGSLEN, stdin) == NULL)
			perror("fgets ");
		else {	
			//main index to user input character array (shell command)
			int i = 0;
			//index to single arg array (buffer)
			int j = 0;
			//index to arglist array (where args are stored after being parsed)
			int k = 0;
			//remove leading whitespace	
			for(; (string1[i] == ' ') || (string1[i] == '\t'); i++);

			//read string until newline is found
			while( string1[i] != '\n' ) {
				//read the arg until whitespace is found (or newline)
				while ( (string1[i] != ' ') && (string1[i] != '\t') && (string1[i] != '\n') ) 
					//copy the arg to array
					string2[j++] = string1[i++];
						
				//add terminating character to end of string
				string2[j] = '\0';
		
				//copy the arg to the arglist
				buf = malloc(j+1);	//allocate j+1 bytes to buf
				strcpy(buf, string2); //copies j+1 bytes from string2 to buf
				arglist[k++] = buf;  //add buf to arglist

				//increment i past whitespace	
				for(; (string1[i] == ' ') || (string1[i] == '\t'); i++);
				
				//reset single arg index
				j = 0;
			}
			arglist[k] = NULL;
			printargs(arglist);
			execvp(arglist[0], arglist);
			perror("execvp failed: ");
			exit(1);
		}
	}
}
/*
 * new process takes a nap and then exits
 */


void child_code(int delay)
{


//	string1 = malloc(sizeof(


	printf("child %d here. will sleep for %d seconds\n", getpid(), delay);
//	kill(getpid(), SIGTERM);
	sleep(delay);
	printf("child done. about to exit\n");
	exit(17);
}
/*
 * parent waits for child then prints a message
 */
void parent_code(int childpid)
{
	int wait_rv;
	int m;		/* return value from wait() */
	
	wait_rv = wait(&m);
	printf("done waiting for %d. Wait returned: %d\nsignal returned: %x\n", childpid, wait_rv, m);

	printf("m: "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN"\n",    			BYTE_TO_BINARY(m>>8), BYTE_TO_BINARY(m));
	//printf("%ld\n",sizeof(int));


}

