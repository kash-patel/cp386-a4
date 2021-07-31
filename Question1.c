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

int readAvailable(char *argv[]);
int readMax();
int initialiseAllocated();
int initialiseNeed();
int commandLoop();
int command_Status();
int printAvailable();
int printMax();
int printAllocated();
int printNeed();


int main (int argc, char *argv[]) {

  readAvailable(argv);
  readMax();
  initialiseAllocated();
  initialiseNeed();

  printf("Number of Customers: %d\n", numProcesses);
  printf("Currently Available resources: ");
  printAvailable();
  printf("Maximum resources from file:\n");
  printMax();

  commandLoop();

  free(available);
  for (size_t i = 0; i < numProcesses; i++) {
    free(max[i]);
    free(allocated[i]);
    free(need[i]);
  }
  free(max);
  free(allocated);
  free(need);

  return 0;
}


int commandLoop () {

  char command[100];
  const char* exitCommand = "Exit";

  printf("Enter Command: ");
  scanf("%s", command);

  while (strcmp(command, exitCommand) != 0) {
    
    if (!strcmp(command, "Status")) command_Status();
    
    printf("Enter Command: ");
    scanf("%s", command);
  }

  return 0;
}

int command_Status () {

  printf("Available Resources:\n");
  printAvailable();
  printf("Maximum Resources:\n");
  printMax();
  printf("Allocated Resources:\n");
  printAllocated();
  printf("Need Resources:\n");
  printNeed();

  return 0;
}

int printAvailable () {
  
  for (size_t i = 0; i < 4; i++) {
    printf("%d ", available[i]);
  } printf("\n");

  return 0;
}

int printMax () {
  
  for (size_t i = 0; i < numProcesses; i++) {
    for (size_t ii = 0; ii < numResourcesTypes; ii++) {
      printf("%d ", max[i][ii]);
    } printf("\n");
  }
  
  return 0;

}

int printAllocated () {
  
  for (size_t i = 0; i < numProcesses; i++) {
    for (size_t ii = 0; ii < 4; ii++) {
      printf("%d ", allocated[i][ii]);
    } printf("\n");
  }

  return 0;
}

int printNeed () {

  for (size_t i = 0; i < numProcesses; i++) {
    for (size_t ii = 0; ii < 4; ii++) {
      printf("%d ", need[i][ii]);
    } printf("\n");
  }

  return 0;
}

int readAvailable (char *argv[]) {

  available = (int *) malloc(4 * sizeof(int));

  for (size_t i = 1; i < 5; i++) {
    available[i - 1] = atoi(argv[i]);
  }

  return 0;
}

int readMax () {

  FILE *in = fopen("./sample4_in.txt", "r");

  if (!in) {
    printf("ERROR: Couldn't open input file.\n");
    return -1;
  }

  max = (int **) malloc(5 * sizeof(int *)); // Initialise to 5 processes for now
  char line[100];

  while (fgets(line, sizeof(line), in)) {

    if (numProcesses > 5) realloc(max, numProcesses * sizeof(int *));

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

int initialiseAllocated () {

  allocated = (int **) malloc(numProcesses * sizeof(int *));

  for (size_t i = 0; i < numProcesses; i++) {
    
    allocated[i] = (int *) malloc(4 * sizeof(int));
    
    for (size_t ii = 0; ii < 4; ii++) {
      allocated[i][ii] = 0;
    }
  }

  return 0;

}

int initialiseNeed () {

  need = (int **) malloc(numProcesses * sizeof(int *));

  for (size_t i = 0; i < numProcesses; i++) {

    need[i] = (int *) malloc(4 * sizeof(int));

    for (size_t ii = 0; ii < 4; ii++) {
      need[i][ii] = max[i][ii] - allocated[i][ii];
    }
  }

  return 0;

}
