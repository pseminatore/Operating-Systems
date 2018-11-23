/*
Class: CPSC 346-01
Patrick Seminatore
Pgm Name: proj7.c 
Pgm Desc: To demonstrate how to pass multiple arguments to a thread using struct
         and how to decompose a problem into threads
Usage: ./a.out
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>


void* writeEnd(void*);
void* readEnd(void*);
int is_prime(int);


struct params
{
  int numToGenerate;
  int randNum;
  pthread_t threadNum1, threadNum2, threadNum3;
  int fd[2];  
  char* charNum;
  int threadID;
};


typedef struct params param;
 
int main(int argc, char* argv[])
{
  //1 writer thread, 3 readers
  pthread_t w1, r1, r2, r3;
  param p1;
  int status = 0;
  p1.charNum = (char*)calloc(8, sizeof(char*));


  //creating ends of pipe
  if (pipe(p1.fd) < 0)
  {      
    printf("Error creating pipe\n");
    exit(-1);
  }

  //the number of random numbers to generate
  int numGen = 0;

  //Get amount of random numbers to generate
  numGen = atoi(argv[1]);

  //set amount in thread parameters
  p1.numToGenerate = numGen;
  

  
  //create writer thread
  status = pthread_create(&w1, NULL, writeEnd, (void*)&p1 );
  if (status != 0)
  {
    printf("Error in thread w1:  %d\n",status);
    exit(-1);
  }
  pthread_join(w1,NULL);

  //create reader thread 1
  status = pthread_create(&r1, NULL, readEnd, (void*)&p1 );
  if (status != 0)
  {
    printf("Error in thread r1:  %d\n",status);
    exit(-1);
  }
  p1.threadNum1 = r1;


  //create reader thread 2
  status = pthread_create(&r2, NULL, readEnd, (void*)&p1 );
  if (status != 0)
  {
    printf("Error in thread r2:  %d\n",status);
    exit(-1);
  }
  p1.threadNum2 = r2;


  //create reader thread 3
  status = pthread_create(&r3, NULL, readEnd, (void*)&p1 );
  if (status != 0)
  {
    printf("Error in thread r3:  %d\n",status);
    exit(-1);
  }
  p1.threadNum3 = r3;

  
  pthread_join(r1,NULL);
  pthread_join(r2,NULL);
  pthread_join(r3,NULL);
  
  return 0; 
}

void* writeEnd(void* param_in)
{
  int randNum;

  param* p = (param*)param_in;
  srand(time(NULL));

    printf("\nNumber of random numbers to generate: %d\n", p -> numToGenerate);

  //Loop to send specified amount of random numbers to pipe
  for (int i = 0; i < p -> numToGenerate; i++)
  {
    //generate random number
    randNum = rand() % 99999999 + 10000000;
    sprintf(p -> charNum, "%d", randNum);

    printf("Randomized Number from writer: %d\n", randNum);    

   if(write(p -> fd[1], p -> charNum, sizeof(p -> charNum)) < 0)
   {
     printf("Error writing message\n");
   }
     // close(p -> fd[1]);

  }
 // wait(NULL);
  printf("\n");
  close(p -> fd[1]);
  pthread_exit(NULL);
}

void* readEnd(void* param_in)
{
  //sleep(1);
  param* p = (param*)param_in;

  close(p-> fd[1]);

  int tempNum = 0;
  int size;
  char numIn[8];
  

  while(size = read(p -> fd[0], numIn, sizeof(numIn)) > 0)
  {
    //get thread ID
    pthread_t tempID = pthread_self();
    
    //determine which thread is running
    if(tempID == p -> threadNum1)
    {
      p -> threadID = 1;
    }
    else if (tempID == p -> threadNum2)
    {
      p -> threadID = 2;
    }
    else if (tempID == p -> threadNum3)
    {
      p -> threadID = 3;
    }
    tempNum = atoi(numIn);
    printf("Reader %d: \t%d is here!\n", p -> threadID, tempNum);

    if(is_prime(tempNum))
    {
      printf("Reader %d: \t%d is prime!\n", p -> threadID, tempNum);
    }
    //sleep(1);
  }
  if (size != 0)
    exit(0);
  pthread_exit(NULL);
  

}


 
int is_prime(int num)
{
  if (num <= 1)
  {
    return 0;
  }

  if (num % 2 == 0 && num > 2)
  { 
    return 0;
  }

  for(int i = 3; i < num / 2; i+= 2)
  {
    if (num % i == 0)
    return 0;
  }

  return 1;
}

