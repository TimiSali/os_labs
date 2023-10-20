#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

char PARALLEL_PATH[] = "./parallel/parallel";

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            if (seed <= 0) {
              printf("seed should be a positive number\n");
              return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            if (array_size <= 0) {
              printf("array size should be a positive number\n");
              return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            if (pnum <= 0) {
              printf("number of parallels should be a positive number\n");
              return 1;
            }
            break;
          case 3:
            with_files = true;
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  int fd[pnum][2];
for (int i = 0; i < pnum; ++i) {
  pipe(fd[i]);
}

  for (int i = 0; i < pnum; ++i) {
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process
        // parallel somehow
        struct MinMax min_max = GetMinMax(array, i*(array_size)/pnum, (i+1)*(array_size)/pnum);
        char* result = malloc(sizeof(char) * (sizeof(int) * 2 + 1));
        sprintf(result, "%d\n%d", min_max.min, min_max.max);
        if (with_files) {
          char *filename = malloc(sizeof(char)*(sizeof(PARALLEL_PATH) + sizeof(int)));
          sprintf(filename, "%s%d", PARALLEL_PATH, i);
          FILE *fp;
          fp = fopen(filename, "w");
          free(filename);
          fprintf(fp, "%s", result);
          fclose(fp);
        } else {
          // use pipe here
          close(fd[i][0]);
          write(fd[i][1], result, strlen(result));
          exit(0);
        }
        free(result);
        return 0;
      } else {
        wait(NULL);
          
      }
    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }

  while (active_child_processes > 0) {
    active_child_processes -= 1;
  }
/*
  while (true) {
    int status;
    pid_t done = wait(&status);
    if (done == -1) {
        break; // no more child processes
    }
  }
  */
  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
      char *filename = malloc(sizeof(char)*(sizeof(PARALLEL_PATH) + i / 10));
      sprintf(filename, "%s%d", PARALLEL_PATH, i);
      FILE *fp;
      fp = fopen(filename, "r");
      fscanf(fp, "%d\n%d", &min, &max);
      fclose(fp);
      remove(filename);
      free(filename);
    } else {
      // read from pipes
      char readbuffer[80];
      close(fd[i][1]);
      read(fd[i][0], readbuffer, sizeof(readbuffer));
      sscanf(readbuffer, "%d\n%d", &min, &max);
    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}