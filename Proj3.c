/*
Class: CPSC 346-01
Patrick Seminatore
Pgm Name: proj3.c 
Pgm Desc: exploraiton of the proc file system 
Usage: 1) standard:  ./a.out -s 
Usage: 2) history:  ./a.out -h 
Usage: 3) load:  ./a.out -l
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void standard();
void history();
void load();


{
 if (argc != 2)
  {
   fprintf(stderr, "Error: Options required\n"); 
   fprintf(stderr, "usage: ./a.out -s|-h|-l\n\n"); 
   exit(EXIT_FAILURE);
  }
  
 if (!strcmp(argv[1],"-s"))
  standard();
 else if (!strcmp(argv[1],"-h"))
  history();
 else if (!strcmp(argv[1],"-l")){
  load();
  }else {
    fprintf(stderr, "Error: Please enter a valid argument\n");
  }  
}

/*
pre: none
post: displays CPU vendor_id, model name, and OS version
*/
void standard()
{
 char ch;
 FILE* ifp;
 char str[80];

 /*
 I've deliberately used two different mechanisms for writing to the console.  
 Use whichever suits you.
 strstr locates a substring
 */

 ifp = fopen("/proc/cpuinfo","r");
 while (fgets(str,80,ifp) != NULL)
  if (strstr(str,"vendor_id") || strstr(str,"model name"))
   puts(str); 
 fclose (ifp);

 ifp = fopen("/proc/version","r");
 while ((ch = getc(ifp)) != EOF)
  putchar(ch);
 fclose (ifp);
}

/*
pre: none
post: displays time since the last reboot (DD:HH:MM:SS), time when the system 
      was last booted, number of processes that have been created since the
      last reboot.  
      Hint: strftime could be useful
*/
void history()
{
char ch;
time_t rawtime;
struct tm *info;
FILE* ifp;
char buffer[80];
char str[80];
char * ptr;
float uptime;
ifp = fopen("/proc/uptime","r");
fscanf(ifp, "%s", str);
uptime = strtof(str, &ptr);
int uptimeMin = uptime / 60;
int uptimeHr = uptimeMin / 60;
int uptimeDays = uptimeHr / 24;
uptimeHr = uptimeHr % 24;
uptimeMin = uptimeMin % 60;
int uptimeSec = uptime / 1;
uptimeSec = uptimeSec % 60;
printf("\nTime since last reboot:   %02d:%02d:%02d:%02d\n\n", uptimeDays, uptimeHr, uptimeMin, uptimeSec);

ifp = fopen("/proc/stat","r");
 while (fgets(str,80,ifp) != NULL)
  if (strstr(str,"processes")){
    puts(str);  
  }

struct tm * timeinfo;
char timeString[80];
int i;
int d = 0;

time ( &rawtime );
timeinfo = localtime ( &rawtime );
strftime(timeString, 80, "%X", timeinfo);
char bootDays[3];
char bootHr[3];
char bootMin[3];
char bootSec[3];

for (i = 0; i < 2; i++){
  bootHr[2] = "\0";  
  bootHr[i] = timeString[d];
  d++;
}
d++;

for (i = 0; i < 2; i++){
  bootMin[2] = "\0";  
  bootMin[i] = timeString[d];
  
  d++;
}
d++;

for (i = 0; i < 2; i++){
  bootSec[2] = "\0"; 
  bootSec[i] = timeString[d];  
 d++;
}
char * endPtr
int boottimeDays = strtol(bootDays, &endPt,10);
int boottimeHr = strtol(bootHr, &endPtr,10)
int boottimeMin = strtol(bootMin, &endPtr,10)
int boottimeSec = strtol(bootSec, &endPtr,10);
boottimeDays = boottimeDays-uptimDays;
boottimeHr = boottimeHr - uptimeHr
boottimeMin = boottimeMin - uptimeMin
boottimeSec = boottimeSec - uptimeSec;
int carry;
if (boottimeMin <0){
  boottimeHr -= 1;  carry = boottimeMin + 60;
 boottimeMin = 60 - carry;
}
if (boottimeSec <0){
  boottimeMin -= 1
  carry = boottimeSec + 60
  boottimeSec = 60 - carry;}
printf("Time of last reboot:   %02d:%02d:%02d\n\n", boottimeHr, boottimeMin, boottimeSec);

 fclose (ifp);
}

/*
pre: none
post: displays total memory, available memory, load average (avg. number of processes over the last minute) 
*/
void load()

  char ch;  
FILE* ifp;
    char str[80];
  int marker = 0;

  ifp = fopen("/proc/meminfo","r");  
 while (fgets(str,80,ifp) != NULL)
      if (strstr(str,"MemTotal") || strstr(str,"MemAvailable"))

     puts(str); 
  ifp = fopen("/proc/loadavg", "r");
printf("%s","Load Average: " );
    while (marker < 4){
      ch = getc(ifp);
      putchar(ch);

      marker++;  }
  putchar('\n');

  fclose (ifp);
}
