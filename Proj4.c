/*
Class: CPSC 346-01
Patrick Seminatore
Pgm Name: proj4.c 
Pgm Desc: Outline of a shell
Usage: ./a.out
*/

#include <stdio.h>
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_LINE  41 
#define TRUE 80

char** getInput();
char** parseInput(char*);
void dispOutput(char**);
char **history;
int marker; 
int counter;


int main(int argc, char* argv[])
{
  //A pointer to an array of pointers to char.  In essence, an array of 
  //arrays.  Each of the second level arrays is a c-string. These hold
  //the command line arguments entered by the user.
  //as exactly the same structure as char* argv[]
  //marker = 0;
  char **args;
  history = malloc(20 * sizeof(char*));
  marker = 0;
  counter = 0;
  char *histString = malloc(20 * sizeof(char*));
  char *histN = malloc(20 * sizeof(char*));
  int histNum;
  char *histCom = malloc(20 * sizeof(char*));
  
  while (TRUE)
  {
    printf("myShell> ");
    fflush(stdout);
    args = getInput();
    int pid;
    char **ampersand = args;
    int index = 0;

    //dispOutput(args);

    histCom = args[0];
    *histN = histCom[0];
    

    //if the user has entered "quit" break out of the loop.
    if (!strcmp(args[0], "quit"))
    {
        break;
        exit(0);
    }
    //if the user has entered "history" display the last 10 commands entered.
    if (!strcmp(args[0], "history"))
    {
        counter = 0;
        while (history[counter] != NULL)
        {
            printf("%d: %s\n", counter+1, history[counter]);
            counter++;
        }
        wait(NULL);          
    }
    //if the user has entered "!!" run the most recent command.
    else if (!strcmp(args[0], "!!"))
    {
        
        histString = history[marker - 2];
        args = parseInput(histString);

    }
    else if (!strcmp(histN, "!"))
    {
        *histN = histCom[1];
        histNum = atoi(histN);
        if ((histNum > marker - 1)||(histNum < 0))
        {
            fprintf(stderr, "Please enter a valid command number\n");
        }
        else 
        {
            histString = history[histNum - 1];
            args = parseInput(histString);
        }
    }
    while (ampersand[index] != NULL)
    {
        if (!strcmp(ampersand[index], "&"))
        {
           // wait(NULL);
           args = parseInput("idle-python2.7 &");
        }
        index++;
    }
    pid = fork();
    if(pid < 0)
    {
        fprintf(stderr, "Fork Error");
    } 
    else if (pid == 0)
    {
        execvp(args[0], args);
        exit(0);
    } 
    else           //parent process
    {
        wait(NULL);
        kill(pid, SIGKILL);
    }
  }
   return 0;
}



/*

Reads input string from the key board.   

invokes parseInput and returns the parsed input to main

*/

char** getInput()
{
    char** parsedInput;
    char buffer[MAX_LINE];
    fflush(stdin);
    fgets(buffer, sizeof(buffer), stdin);
    if (marker >=9)
    {
        for (int i = 0; i > 9; i++)
        {
            history[i] = history[i+1];
        }
        history[marker] = strdup(buffer);
        
    }
    else 
    {
        history[marker] = strdup(buffer);
        marker++;        
    }
    parsedInput = parseInput(buffer);
    return parsedInput;
}  



/*
inp is a cstring holding the keyboard input
returns an array of cstrings, each holding one of the arguments entered at
the keyboard. The structure is the same as that of argv
Here the user has entered three arguements:
myShell>cp x y
*/ 

char** parseInput(char* inp)
{
    char** args = malloc(10 * sizeof(char*));
    int toknum = 0;
    const char delimiters[] = "  \n";
    char *token = strdup(strtok(inp, delimiters));
    while (token != NULL)
    {
        args[toknum] = token;     
        toknum++; 
        token = strtok(NULL, delimiters);
    }
    free(token);
    token = NULL;
    return args;
} 

 

/*

Displays the arguments entered by the user and parsed by getInput

*/

void dispOutput(char** args)
{
  
  int index = 0;
  while (args[index] != NULL)
  {
    printf("%d: %s\n", index, args[index]);
    index++;
  } 
  printf("\n%s\n", "HISTORY:");
  while (history[marker] != NULL)
  {
    printf("%d: %s\n", marker, history[marker]);
    marker++;
  }
}
