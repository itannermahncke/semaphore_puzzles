#include <pthread.h>
#include <semaphore.h>

#include "puzzle.h"

int main(void) {
  start_threads(42);
  return 0;
}
