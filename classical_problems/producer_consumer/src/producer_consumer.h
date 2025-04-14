#pragma once

#include <pthread.h>
#include <semaphore.h> // sem_t
#include <stddef.h>    // size_t

// A set of arguments to be passed to each thread.
typedef struct {
  sem_t *lock;             // control buffer access
  sem_t *block_when_empty; // indicate if buffer is empty
  sem_t *block_when_full;  // indicate if buffer is full
} thread_args;

typedef struct {
  size_t id;              // consumer id
  size_t item_quota;      // num items to consume
  size_t *items_consumed; // num items consumed
  thread_args t_args;     // threadwide semaphores
} consumer_args;

typedef struct {
  size_t id;              // producer ids
  size_t item_quota;      // num items to produce
  size_t *items_produced; // num items produced
  thread_args t_args;     // threadwide semaphores
} producer_args;

/**
 * Consume a set number of items from the buffer as they become available.
 *
 * This function is called by each instance of a consumer thread. The consumer
 * should take items from the buffer as they become available. if no items are
 * available, the consumer thread is blocked from acting until more appear. The
 * thread ends when the consumer is satisfied.
 *
 * @param args A thread_args pointer to the function arguments.
 * @return The null pointer.
 */
void *consumer(void *args);

/**
 * Produce a set number of items to place in the buffer.
 *
 * This function is called by each instance of a producer thread. The producer
 * should place items in the buffer on a set interval. The thread ends when the
 * expected number of items have been produced.
 *
 * @param args A thread_args pointer to the function arguments.
 * @return The null pointer.
 */
void *producer(void *args);

/**
 * Start up a pool of producers and consumers.
 *
 * Given a number of producers and consumers to start up, create a pool of
 * threads behaving as one or the other. This function manages variables shared
 * between all threads, including the buffer that producers add items to and
 * consumers take from. Finally, this function cleans up all threads when
 * finished.
 *
 * @param num_producers The number of producers in the pool.
 * @param num_consumers The number of consumers in the pool.
 * @param producer_quota The number of items each producer adds to the buffer.
 * @param num_consumers The number of items each consumer takes from the buffer.
 */
void start_threads(const size_t num_producers, const size_t num_consumers,
                   const size_t producer_quota, const size_t consumer_quota,
                   const size_t buffer_size);
