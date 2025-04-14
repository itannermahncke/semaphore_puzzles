#include "producer_consumer.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

void *producer(void *args) {
  // grab the args
  producer_args *p_args = (producer_args *)args;
  printf("Producer %zu woke up!\n", p_args->id);

  for (size_t i = 0; i < p_args->item_quota; ++i) {
    // Produce an item
    int item = i;
    printf("Producer %zu made an item!\n", p_args->id);

    // Attempt to place an item in the buffer
    sem_wait(p_args->t_args.block_when_full);

    // Edit the buffer
    sem_wait(p_args->t_args.lock);
    // if we had a real queue, place the item here
    (*p_args->items_produced)++;
    sem_post(p_args->t_args.lock);

    // Indicate that another item is in the buffer
    sem_post(p_args->t_args.block_when_empty);

    sleep(1);
  }

  return NULL;
}

void *consumer(void *args) {
  // grab the args
  consumer_args *c_args = (consumer_args *)args;
  printf("Consumer %zu woke up!\n", c_args->id);

  for (size_t i = 0; i < c_args->item_quota; ++i) {
    // Attempt to take an item
    sem_wait(c_args->t_args.block_when_empty);

    // Edit the buffer
    sem_wait(c_args->t_args.lock);
    // If we had a real queue, take the item here
    (*c_args->items_consumed)++;
    sem_post(c_args->t_args.lock);

    // Process the item here
    printf("Consumer %zu took item %zu!\n", c_args->id, i);

    // Indicate that an item has been taken
    sem_post(c_args->t_args.block_when_full);
  }

  return NULL;
}

void start_threads(const size_t num_producers, const size_t num_consumers,
                   const size_t producer_quota, const size_t consumer_quota,
                   const size_t buffer_size) {
  // create space for all of the threads
  pthread_t *producer_mem = malloc(sizeof(pthread_t) * num_producers);
  pthread_t *consumer_mem = malloc(sizeof(pthread_t) * num_consumers);
  producer_args *producer_arg_mem =
      malloc(sizeof(producer_args) * num_producers);
  consumer_args *consumer_arg_mem =
      malloc(sizeof(consumer_args) * num_consumers);

  // semaphores
  sem_t lock; // control edit access to the buffer
  sem_init(&lock, 0, 1);
  sem_t block_when_empty; // bar consumption if buffer is empty
  sem_init(&block_when_empty, 0, 0);
  sem_t block_when_full; // bar production if buffer is full
  sem_init(&block_when_full, 0, buffer_size);

  // create a semap

  // Define the counters
  size_t items_produced = 0;
  size_t *items_produced_ptr = &items_produced;
  size_t items_consumed = 0;
  size_t *items_consumed_ptr = &items_consumed;

  // construct the producers
  for (size_t i = 0; i < num_producers; ++i) {
    // construct the argument struct
    producer_args *arg = &producer_arg_mem[i];
    arg->id = i;
    arg->items_produced = items_produced_ptr;
    arg->t_args.lock = &lock;
    arg->t_args.block_when_empty = &block_when_empty;
    arg->t_args.block_when_full = &block_when_full;
    arg->item_quota = producer_quota;

    // run the thread
    pthread_create(&producer_mem[i], NULL, producer, (void *)arg);
  }

  // construct the consumers
  for (size_t i = 0; i < num_consumers; ++i) {
    // construct the argument struct
    consumer_args *arg = &consumer_arg_mem[i];
    arg->id = i;
    arg->items_consumed = items_consumed_ptr;
    arg->t_args.lock = &lock;
    arg->t_args.block_when_empty = &block_when_empty;
    arg->t_args.block_when_full = &block_when_full;
    arg->item_quota = consumer_quota;

    // run the thread
    pthread_create(&consumer_mem[i], NULL, consumer, (void *)arg);
  }

  // close the threads
  for (size_t i = 0; i < num_producers; ++i) {
    pthread_join(producer_mem[i], NULL);
  }
  for (size_t i = 0; i < num_consumers; ++i) {
    pthread_join(consumer_mem[i], NULL);
  }

  // clean up memory
  free(producer_mem);
  free(consumer_mem);

  // close the semaphores
  sem_destroy(&lock);
  sem_destroy(&block_when_empty);
  sem_destroy(&block_when_full);

  // print results
  printf("Given %zu producers that make %zu items each, a proper solution "
         "produces %zu total items.\nA total of %zu items were produced.\n",
         num_producers, producer_quota, num_producers * producer_quota,
         items_produced);
  printf("Given %zu consumers that consumes %zu items each, a proper solution "
         "consumes %zu total items.\nA total of %zu items were consumed.\n",
         num_consumers, consumer_quota, num_consumers * consumer_quota,
         items_consumed);
}
