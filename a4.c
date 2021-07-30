#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <time.h>


int __main__ (int argc, char *argv[]) {
  readFile(argv[1]);
}

int readFile (char *filename) {

  FILE *in = fopen(filename, "r");

  if (!in) {

    printf("ERROR: Couldn't open input file.\n");
    return -1;
  }

  char line[100];

  while (fgets(line, sizeof(line), in)) {
    printf("%s", line);
  }

  fclose(in);
  
}

int main (int argc, char *argv[]) {
  __main__(argc, argv);
  return 0;
}