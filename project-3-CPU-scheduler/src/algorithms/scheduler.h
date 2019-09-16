#pragma once
#include "types/event.h"
#include "types/scheduling_decision.h"
#include "types/thread.h"


/**
 * Abstract base class for all scheduling algorithms.
 */
struct Scheduler {
  /**
   * Returns the next thread that should be executed, or NULL if there is no
   * thread available to run.
   */
  virtual SchedulingDecision* get_next_thread(const Event* event) = 0;

  /**
   * Enqueues the given thread in this scheduler's ready queues.
   */
  virtual void enqueue(const Event* event, Thread* thread) = 0;

  /**
   * Returns true if the currently running thread should be preempted upon the
   * arrival of the given thread, as represented by event.
   */
  virtual bool should_preempt_on_arrival(const Event* event) const = 0;

  /**
   * Returns the number of threads in this scheduler's ready queues.
   */
  virtual size_t size() const = 0;

  /**
   * Returns true if the scheduler has no threads in its ready queues.
   */
  bool empty() const { return size() == 0; }

  /**
   * Virtual destructor (as a best practice).
   */
  virtual ~Scheduler() {}
};
