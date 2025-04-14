#pragma once

#include <pthread.h>    // pthread_mutex_t
#include <semaphore.h>  // sem_t
#include <stddef.h>     // size_t

/// A set of arguments to be passed to each thread.
typedef struct {
  /// The lock to control access to the number of ready threads.
  pthread_mutex_t* lock;
  /// The semaphore used to signal when all threads are ready.
  sem_t* go_signal;
  /// The number of threads that have completed the ready() call.
  size_t* ready_threads;
  /// A number to identify the running thread.
  size_t thread_id;
  /// The total number of threads in the pool.
  size_t total_threads;
} thread_args;

/**
 * Print the message "Thread X ready!" to standard output (X is the thread ID).
 *
 * @param thread_id The identifier of the thread.
 */
void ready(size_t thread_id);

/**
 * Print the message "Thread X go!" to standard output (X is the thread ID).
 *
 * @param thread_id The identifier of the thread.
 */
void go(size_t thread_id);

/**
 * Run ready(), then go() with appropriate concurrency controls.
 *
 * When a pool of threads each runs this function with the appropriate sets of
 * arguments, each instance of this function runs ready(), then go() such that
 * each thread in the pool finishes running ready() before any of the threads
 * run go().
 *
 * @param args A thread_args pointer to the function arguments.
 * @return The null pointer.
 */
void* synchronize(void* args);

/**
 * Start up a pool of threads that run synchronize().
 *
 * Given a number of threads to start up, create a pool of threads that run
 * synchronize() with the appropriate arguments. These arguments include shared
 * variables among threads, which this function manages, as well as some
 * thread-local arguments. After spawning these threads, wait until each thread
 * is complete.
 *
 * @param num_threads The number of threads in the pool.
 */
void start_threads(size_t num_threads);
