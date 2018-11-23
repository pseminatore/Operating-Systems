/*
Class: CPSC 346-01
Patrick Seminatore
Pgm Name: proj5.c 
Pgm Desc: Exploration of Peterson algorithm for critical sections
Usage: ./a.out 1 1 1 1
        OR
       ./a.out
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

void parent(int time_crit_sect, int time_non_crit_sect, int pr_p, int* turn, int shmid);
void child(int time_crit_sect, int time_non_crit_sect, int pr_c, int* turn, int shmid);
void cs(char process, int time_crit_sect);
void non_crit_sect(int time_non_crit_sect);



void main(int argc, char* argv[])

{

 //check for proper arguments
 int time_child;
 int time_child_non_cs;
 int time_parent;
 int time_parent_non_cs;
 int pid;
 int shmid;
 //char* address;
 int *turn;
 int pr_p = 0;
 int pr_c = 1;


 if (argc == 1)
 {
     time_child = 1;
     time_child_non_cs = 1;
     time_parent = 1;
     time_parent_non_cs = 1;
 }
 else if (argc == 5)
 {
     time_parent = atoi(argv[1]);
     time_child = atoi(argv[2]);
     time_parent_non_cs = atoi(argv[3]);
     time_child_non_cs = atoi(argv[4]);
 }
 else
 {
     fprintf(stderr, "Please double check your parameters and try again!\n");
     exit(0);
 }
 
 shmid = shmget(0, 1, 0777 | IPC_CREAT);

 turn = (int*)shmat(shmid, 0, 0);

*turn = 0;

 
 //fork here
pid = fork();

 //parBegin
if (pid == 0)
 {
  child(time_child, time_child_non_cs, pr_c, turn, shmid);
 }
else if (pid > 0)
{
  parent(time_parent, time_parent_non_cs, pr_p, turn, shmid);
}
else 
{
    fprintf(stderr, "Fork Error");
    exit(0);
}
 //parEnd

}



void parent(int time_crit_sect, int time_non_crit_sect, int pr_p, int* turn, int shmid)

{
 for (int i = 0; i < 5; i++)
  {
    sleep(6);   
    //Peterson 
    pr_p = 1;
    (*turn) = 1;

    if (i == 3)
    {
        sleep(4);
    }

    //protect this
    while((pr_p == 1) && *turn)
    {
        cs('p', time_crit_sect);
        (pr_p) = 0; 
        sleep(1);     
    }
  }

  non_crit_sect(time_non_crit_sect); 
  wait(NULL);
  shmdt(turn);  
  shmctl(shmid, IPC_RMID, 0);    
  
}


void child(int time_crit_sect, int time_non_crit_sect, int pr_c, int* turn, int shmid)

{   
    
 for (int i = 0; i < 5; i++)
  {
    sleep(4); 

    //Peterson
    pr_c = 1;
    (*turn) = 0;

    if (i == 1)
    {
        sleep(4);
    }
    
    wait(NULL);   
    //protect this
    while((pr_c == 1) && !(*turn))
    {
        cs('c', time_crit_sect);
        (pr_c) = 0;  
        sleep(2);     
    }
  }
  non_crit_sect(time_non_crit_sect);  
  exit(1);
}



void cs(char process, int time_crit_sect)

{

 if (process == 'p')

  {

   printf("parent in critical sction\n");

   sleep(time_crit_sect);

   printf("parent leaving critical section\n");

  }

 else

  {

   printf("child in critical sction\n");

   sleep(time_crit_sect);

   printf("child leaving critical section\n");

  }

}



void non_crit_sect(int time_non_crit_sect)

{
 sleep(time_non_crit_sect);
}
