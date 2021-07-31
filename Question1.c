#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <time.h>


int processes = 0;
int resourcesTypes = 0;

int *available;
int **allocated;
int **max;
int **need;
int *safeSeq;

char *inputString;


int readRequests();


int main (int argc, char *argv[]) {

  readRequests();

  return 0;
}


int readRequests () {

  FILE *in = fopen("./sample4_in.txt", "r");

  if (!in) {

    printf("ERROR: Couldn't open input file.\n");
    return -1;
  }

  char line[100];

  while (fgets(line, sizeof(line), in)) {
    printf("%s", line);
  }

  fclose(in);

  return 0;
  
}