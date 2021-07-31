#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

int numProcesses = 0;
int numResourcesTypes = 4;
int *available;
int **allocated;
int **max;
int **need;
int work[4];
int *finish;
int *safeSeq;

int readAvailable(char *argv[]);
int readMax();
int initialiseAllocated();
int initialiseNeed();
int commandLoop();
int command_RQ(char *command);
int command_RL(char *command);
int command_Run();
int command_Status();
int checkSafety();
int allocateResources(int *args);
int releaseResources(int *args);
int getSafeSeq();
int printAvailable();
int printMax();
int printAllocated();
int printNeed();


int main (int argc, char *argv[]) {

  readAvailable(argv);
  readMax();
  initialiseAllocated();
  initialiseNeed();
  finish = (int *) malloc(numProcesses * sizeof(int));
  safeSeq = (int *) malloc(numProcesses * sizeof(int));

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
  free(finish);
  free(safeSeq);

  return 0;
}


int commandLoop () {

  char *command;
  size_t commandSize = 40;
  command = malloc(commandSize * sizeof(char));

  printf("Enter Command: ");
  getline(&command, &commandSize, stdin);

  while (strcmp(command, "Exit\n")) {
    
    if (!strcmp(command, "Status\n")) {
      command_Status();
    } else if (!strcmp(command, "Run\n")) {
      command_Run();
    } else if (command[0] == 'R' && command[1] == 'Q') {
      command_RQ(command);
    } else if (command[0] == 'R' && command[1] == 'L') {
      command_RL(command);
    } else {
      printf("Invalid command format.\n");
    }

    free(command);
    command = malloc(commandSize * sizeof(char));

    printf("Enter Command: ");
    getline(&command, &commandSize, stdin);

  };

  free(command);

  return 0;
}

int command_RQ (char *command) {

  command = strtok(command, "\n");

  int *args = malloc(5 * sizeof(int));
  int numArgs = 0;

  char *token = strtok(command, " ");
  token = strtok(NULL, " ");

  while (token != NULL) {

    args[numArgs] = atoi(token);
    token = strtok(NULL, " ");
    numArgs++;

  }

  for (size_t i = 1; i < 5; i++) {
    if (args[i] > need[args[0]][i - 1]) {
      printf("Customer %d's request exceeds their maximum. Request denied.\n", args[0]);
      return -1;
    }
  }

  for (size_t i = 1; i < 5; i++) {
    if (args[i] > available[i - 1]) {
      printf("Customer %d's request exceeds availability. Request denied.\n", args[0]);
      return -1;
    }
  }

  allocateResources(args);

  if (checkSafety() == -1) {

    releaseResources(args);
    printf("Request leaves the system in an unsafe state. Request denied.\n");

    return -1;
  }

  printf("State is safe, and request is satisfied\n");

  return 0;
}

int command_RL (char *command) {

  command = strtok(command, "\n");

  int *args = malloc(5 * sizeof(int));
  int numArgs = 0;

  char *token = strtok(command, " ");
  token = strtok(NULL, " ");

  while (token != NULL) {

    args[numArgs] = atoi(token);
    token = strtok(NULL, " ");
    numArgs++;

  }

  for (size_t i = 1; i < 5; i++) {
    if (allocated[args[0]][i - 1] < args[i]) {
      printf("Customer %d trying to release more resources than allocated. Release failed.\n", args[0]);
      return -1;
    }
  }

  releaseResources(args);

  printf("The resources have been released successfully\n");

  return 0;
}

int command_Run () {

  if (getSafeSeq() != -1) {
    printf("Safe Sequence is: ");
    for (size_t i = 0; i < numProcesses; i++) {
      printf("%d ", safeSeq[i]);
    } printf("\n");
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

int checkSafety () {
  return getSafeSeq();
}

int allocateResources (int *args) {

  for (size_t i = 0; i < 4; i++) {
    allocated[args[0]][i] += args[i+1];
    available[i] -= args[i+1];
    need[args[0]][i] = max[args[0]][i] - allocated[args[0]][i];
  }

  return 0;
}

int releaseResources (int *args) {

  for (size_t i = 0; i < 4; i++) {
    allocated[args[0]][i] -= args[i+1];
    available[i] += args[i+1];
    need[args[0]][i] = max[args[0]][i] - allocated[args[0]][i];
  }

  return 0;
}

int getSafeSeq () {

  for (size_t i = 0; i < numProcesses; i++) {
    finish[i] = FALSE;
  }

  int finished = 0;

  for (size_t i = 0; i < 4; i++) {
    work[i] = available[i];
  }

  while (finished < numProcesses) {

    int safe = FALSE;

    for (size_t i = 0; i < numProcesses; i++) {
    
       if (finish[i] == FALSE) {

        int possible = TRUE;

        for (size_t ii = 0; ii < 4; ii++) {
          
          if (need[i][ii] <= work[ii]) {
            work[ii] += allocated[i][ii];
          } else {
            possible = FALSE;
            break;
          }
        }

        if (possible == TRUE) {
          
          safeSeq[finished] = i;
          finish[i] = TRUE;
          finished++;
          safe = TRUE;
        }
      }
    }

    if (safe == FALSE) {
      
      for (size_t i = 0; i < numProcesses; i++) {
        safeSeq[i] = -1;
      }

      return -1;
    }
  }

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

    if (numProcesses > 5) max = realloc(max, numProcesses * sizeof(int *));

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
