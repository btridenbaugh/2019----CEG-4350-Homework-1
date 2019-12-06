#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h> //Signal header file to ignore and alter signal commands


// Function: void parse(char *line, char **argv)
// Purpose : This function takes in a null terminated string pointed to by 
//           <line>.  It also takes in an array of pointers to char <argv>.
//           When the function returns, the string pointed to by the 
//           pointer <line> has ALL of its whitespace characters (space, 
//           tab, and newline) turned into null characters ('\0').  The
//           array of pointers to chars will be modified so that the zeroth
//           slot will point to the first non-null character in the string
//           pointed to by <line>, the oneth slot will point to the second 
//           non-null character in the string pointed to by <line>, and so
//           on. In other words, each subsequent pointer in argv will point
//           to each subsequent "token" (characters separated by white space)
//           IN the block of memory stored at the pointer <line>.  Since all
//           the white space is replaced by '\0', every one of these "tokens"
//           pointed to by subsequent entires of argv will be a valid string
//           The "last" entry in the argv array will be set to NULL.  This 
//           will mark the end of the tokens in the string.
//           

void nothing(int signum) { /*DO NOTHING*/ }

void  parse(char *line, char **argv)
{
     // We will assume that the input string is NULL terminated.  If it
     // is not, this code WILL break.  The rewriting of whitespace characters
     // and the updating of pointers in argv are interleaved.  Basically
     // we do a while loop that will go until we run out of characters in
     // the string (the outer while loop that goes until '\0').  Inside
     // that loop, we interleave between rewriting white space (space, tab,
     // and newline) with nulls ('\0') AND just skipping over non-whitespace.
     // Note that whenever we encounter a non-whitespace character, we record
     // that address in the array of address at argv and increment it.  When
     // we run out of tokens in the string, we make the last entry in the array
     // at argv NULL.  This marks the end of pointers to tokens.  Easy, right?
    
     while (*line != '\0') // outer loop.  keep going until the whole string is read
        { // keep moving forward the pointer into the input string until
          // we encounter a non-whitespace character.  While we're at it,
          // turn all those whitespace characters we're seeing into null chars.

          while (*line == ' ' || *line == '\t' || *line == '\n' || *line == '\r')
           { *line = '\0';     
             line++;
           }

          // If I got this far, I MUST be looking at a non-whitespace character,
          // or, the beginning of a token.  So, let's record the address of this
          // beginning of token to the address I'm pointing at now. (Put it in *argv)
          // then we'll increment argv so that the next time I store an address, it 
          // will be in the next slot of the array of integers.
         
          *argv++ = line;          /* save the argument position     */

          // Ok... now let's just keep incrementing the input line pointer until
          // I'm looking at whitespace again.  This "eats" the token I just found
          // and sets me up to look for the next.

            while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n' && *line !='\r') {
               line++;
               //if (*line == '&'){
                   //printf("Running process in the background...\n");
                   
               //}
            }/* skip the argument until ...    */
        }

     // Heh, I ran out of characters in the input string.  I guess I'm out of tokens.
     // So, whatever slot of the array at argv I'm pointing at?  Yeah, put a NULL
     // there so we can mark the end of entries in the table.

     *argv = NULL;                 /* mark the end of argument list  */
}

void execute(char** argv)
{
    int child_status;
    pid_t child_pid; //Creates the pid for the child
    child_pid = fork(); //Creates the child process
    if (child_pid == 0) { //Only the child shall run this execution
        printf("I am the child process\n"); //Output from the child process
        if (argv[1] == NULL)
        {
            printf("I would fork() a process to execute %s if I knew how\n", *argv);
        }
        else
        {
            printf("I would fork() a process to execute %s with the parameters\n", *argv++);
            while (*argv != NULL)
                printf("    %s\n", *argv++);
            printf("if I knew how\n");
        }
        exit(0);
    }
    printf("Parent is waiting on the child to terminate...\n");
    waitpid(child_pid, &child_status, 0); //Waits for the child to terminate
    printf("Child has been terminated, running Parent Process...\n");
}

void bexecute(char** argv)
{ //This is a modified execute function to run the child process in the background.
    int child_status;
	pid_t child_pid; //Creates the pid for the child
	child_pid = fork(); //Creates the child process
	if (child_pid == 0) { //Only the child shall run this execution
		printf("I am the Child Process running in the background...\n"); //Output from the child process
		if (argv[1] == NULL)
		{
			printf("I would fork() a process to execute %s if I knew how\n", *argv);
		}
		else
		{
			printf("I would fork() a process to execute %s with the parameters\n", *argv++);
			while (*argv != NULL)
				printf("    %s\n", *argv++);
			printf("if I knew how\n");
		}
	}
}
     

     
int main(int argc, char** argv, char** envp)
{
   // char background[1] = "&";
    int length; //Variable for the length of the actual string
    char  line[1024];   // This is the string buffer that will hold
                         // the string typed in by the user.  This 
                         // string will be parsed.  The shell will do
                         // what it needs to do based on the tokens it
                         // finds.  Note that a user may NOT type in 
                         // an input line of greater than 1024 characters
                         // because that's the size of the array.
                                     
     char  *largv[64];    // This is a pointer to an array of 64 pointers to
                          // char, or, an array of pointers to strings. 
                          // after parsing, this array will hold pointers
                          // to memory INSIDE of the string pointed to by 
                          // the pointer line.  argv[0] will be the string
                          // version of the first token inside of line... 
                          // argv[1] will be the second... and so on... 
                          // See the routine parse() for details.

     char shell_prompt[15]; // This string will hold the shell prompt string

     // set the default prompt
     strcpy(shell_prompt, "SillyShell");

     // The shell by default goes forever... so... while forever ;)

     while (1) 
       {  printf("%s> ",shell_prompt);  // display the shell prompt
			
		  signal(SIGINT, nothing); // Does nothing with CTRL + c

		  if (fgets(line, 1024, stdin) == NULL) // use the safe fgets() function to read the user's command line
		  { //CTRL + d returns a NULL value for fgets(), so this makes the program exit when CTRL + d is called by the user
			  printf("CTRL+d exit\n");
			  exit(0);
		  }

          line[strlen(line)-1]='\0'; // This is a dirty hack.  Figure it out maybe?
		  

          if (*line != '\0') // If something was actually typed, then do something...
            { // First, get all the addresses of all of the tokens inside the input line
              parse(line, largv);     //   parse the line to break it into token references

              // Check the first token to see if there are any built in commands
              // we want to handle directly.  Do this with an "if/then/else" ladder.
              // if we hit the end of the ladder, we assume the command line was requesting
              // an external program be run as a child process and do that....  
			  
			  if (strcmp(largv[0], "printenv") == 0) { while (*envp != NULL)
													   {
														 printf("%s\n", *envp);
														 envp++;
													   } 
													 } else //Prints out all environment variables in the shell.
              if (strcmp(largv[0], "exit")      == 0) exit(0); else
              if (strcmp(largv[0], "done")      == 0) exit(0); else
              if (strcmp(largv[0], "newprompt") == 0) { if (largv[1] != NULL)
                                                          strncpy(shell_prompt, largv[1], 15); 
                                                        else
                                                          strncpy(shell_prompt, "SillyShell", 15);
                                                     } else
            
             length = strlen(line); //Finds the length of the string
             if (line[length-1]=='&'){ //Compares to the & case to run process in the background
                 printf("Running Child Process in the Background...\n");
                 bexecute(largv); //Runs the background execute command
             }
             else
                 execute(largv);           /* otherwise, execute the command */
            }
     }
}

                

