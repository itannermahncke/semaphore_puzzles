#include "puzzle.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

void ready(size_t thread_id) { printf("Thread %zu ready!\n", thread_id); }

void go(size_t thread_id) { printf("Thread %zu go!\n", thread_id); }

void *synchronize(void *args) {
  // Replace the body of this function with your implementation.
  ready(*(size_t *)args);
  go(*(size_t *)args);
  return NULL;
}

void start_threads(size_t num_threads) {
  // Replace the body of this function with your implementation.
  for (size_t i = 0; i < num_threads; ++i) {
    synchronize((void *)&i);
  }
}
