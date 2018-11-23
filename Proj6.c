/*
Class: CPSC 346-01
Patrick Seminatore
Pgm Name: proj6.c 
Pgm Desc: an opportunity to work with semaphores, one of the most important inter-process communication mechanisms, to solve a classic IPC problem.
Usage: ./a.out
*/


#include <stdio.h>
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>

#define INIT_MUTEX 1 
#define INIT_EMPTY 100 
#define INIT_FULL  0 
#define EMPTY_ARG  0 

typedef struct sembuf sem_struct;

void set_sem_values(int,int,int);
void get_sem_values(int,int,int);
void set_sem_struct(sem_struct[],int);
int create_semaphore(int);
void fatal_error(char[]);
void cleanup(int,int,int);
void producer(int mutex, int empty, int full);
void consumer(int mutex, int empty, int full);
void cleanup(int,int,int);
void criticalSection();

const int PROD = 0;
const int CONS = 1;

/*struct sembuf
{
    int sem_num;
    int sem_op;
    int sem_flg;
  };*/


int main(int argc, char* argv[])

{ 
 int mutex;
 int empty;
 int full; 

 int value;

 
 mutex = create_semaphore(0);
 empty = create_semaphore(1);
 full = create_semaphore(2);

 //set semaphores to initial values
 set_sem_values(mutex, empty, full);


 printf("Initial semaphore values:\n");
 get_sem_values(mutex, empty, full);

 //fork off a process
 if ((value = fork()) < 0)
  fatal_error("fork");
 else
  if (value == 0)  //child
    { 
      consumer(mutex, empty, full);
    }
  else    //parent
    { 
     producer(mutex, empty, full);        
    }

 if (value == 0)
   {
    printf("\nFinal semaphore values:\n");
    get_sem_values(mutex, empty, full); 
    cleanup(mutex,empty,full); //remove semaphores
   }
 return 0;
}
  
  
  

 
void producer(int mutex, int empty, int full)
{
  struct sembuf s_wait[1], s_signal[1];
  int ch_stat, status;
  set_sem_struct(s_wait, 1);
  set_sem_struct(s_signal, -1);

  int i;
  for(i = 0; i < 5; i++)
   {    
     if (semop(mutex, s_wait, 1) == -1)
        fatal_error("mutex");
     if (semop(empty, s_wait, 1) == -1)
        fatal_error("empty");            
    criticalSection(PROD);
    if (semop(mutex, s_signal, 1) == -1)
        fatal_error("mutex");
    if (semop(full, s_signal, 1) == -1)
        fatal_error("full");
   }
   wait(NULL);
}
 
void consumer(int mutex, int empty, int full)
{
    struct sembuf s_wait[1], s_signal[1];
    int ch_stat, status;
    set_sem_struct(s_wait, 1);
    set_sem_struct(s_signal, -1);
    wait(NULL);
  sleep(4);    

    int i;
    for (i = 0; i <5; i++)
     { 
       wait(NULL);
       sleep(1);
      if (semop(mutex, s_wait, 1) == -1)
        fatal_error("mutex");
      if (semop(full, s_wait, 1) == -1)
        fatal_error("full");
      wait(NULL);        
      criticalSection(CONS);
      if (semop(mutex, s_signal, 1) == -1)
        fatal_error("mutex");
      if (semop(empty, s_signal, 1) == -1)
        fatal_error("empty");
      wait(NULL);  
      sleep(1);      
   }   
}

void criticalSection(int who)
{
  if (who == PROD)
    printf("Producer making an item\n");
 else
    printf("Consumer consuming an item\n");
 }

 
void get_sem_values(int mutex, int empty, int full)
{
 int m, e, f; 
 m = semctl(mutex, 0, GETVAL, 0);
 e = semctl(empty, 0, GETVAL, 0);
 f = semctl(full, 0, GETVAL, 0);
 printf("mutex: %d empty: %d full: %d\n", m,e,f);

}


void fatal_error(char sem[])
{
 perror(strcat("Exiting. Failed on ",sem)); 
 exit(1);
}


void cleanup(int mutex, int empty, int full)
{
 semctl(mutex, 1, IPC_RMID, EMPTY_ARG);
 semctl(empty, 1, IPC_RMID, EMPTY_ARG);
 semctl(full, 1, IPC_RMID, EMPTY_ARG);
}


void set_sem_values(int mutex, int empty, int full)
{
 semctl(mutex, 0, SETVAL, INIT_MUTEX);
 semctl(empty, 0, SETVAL, INIT_EMPTY);
 semctl(full, 0, SETVAL, INIT_FULL);
}


void set_sem_struct(sem_struct sem[], int op_val)
{
 sem[0].sem_num = 0;
 sem[0].sem_op = op_val;
 sem[0].sem_flg = SEM_UNDO;
} 


int create_semaphore(int key)
{
 int new_sem;
 if ((new_sem = semget(key, 1, 0777 | IPC_CREAT)) == -1)
   {
    perror("semget failed");
    exit(1);
   }
 return new_sem;
}
