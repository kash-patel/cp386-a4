#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <time.h>


int numProcesses = 0;
int numResourcesTypes = 4;

int *available;
int **allocated;
int **max;
int **need;
int *safeSeq;

char *inputString;


int readResources(char *argv[]);
int readRequests();


int main (int argc, char *argv[]) {

  readResources(argv);
  readRequests();

  printf("Number of Customers: %d\n", numProcesses);
  printf("Currently Available resources: ");
  for (size_t i = 0; i < argc - 1; i++) {
    printf("%d ", available[i]);
  } printf("\n");
  printf("Maximum resources from file:\n");
  for (size_t i = 0; i < numProcesses; i++) {
    for (size_t ii = 0; ii < numResourcesTypes; ii++) {
      printf("%d ", max[i][ii]);
    } printf("\n");
  }
  

  return 0;
}


int readResources (char *argv[]) {

  available = (int *) malloc(4 * sizeof(int));

  for (size_t i = 1; i < 5; i++) {
    available[i - 1] = atoi(argv[i]);
  }

  return 0;
}


int readRequests () {

  FILE *in = fopen("./sample4_in.txt", "r");

  if (!in) {

    printf("ERROR: Couldn't open input file.\n");
    return -1;
  }

  max = (int **) malloc(5 * sizeof(int *)); // Initialise to 5 processes for now
  char line[100];

  while (fgets(line, sizeof(line), in)) {

    max[numProcesses] = (int *) malloc(4 * sizeof(int));
    
    char *tempLine = (char *) malloc(sizeof(line) * sizeof(char));
    strcpy(tempLine, line);
    char *token = strtok(tempLine, ",");

    int i = 0;

    while (token != NULL) {

      max[numProcesses][i] = atoi(token);

      token = strtok(NULL, ",");

      i++;

    }

    numProcesses++;
  }

  fclose(in);

  return 0;

}