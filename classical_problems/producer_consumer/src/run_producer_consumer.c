#include "producer_consumer.h"
#include <pthread.h>
#include <semaphore.h>

int main(void) {
  const size_t num_producers = 3;
  const size_t num_consumers = 3;
  const size_t producer_quota = 3;
  const size_t consumer_quota = 3;
  const size_t buffer_size = 2;
  start_threads(3, 3, 3, 3, 2);
  return 0;
}
