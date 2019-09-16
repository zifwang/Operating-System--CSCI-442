#pragma once
#include "types/scheduling_decision.h"
#include "types/thread.h"


/**
 * Represents the various types of events that can happen in the simulation.
 */
struct Event {
  /**
   * The type of the event.
   */
  enum Type {
    /**
     * A thread was created in the system.
     */
    THREAD_ARRIVED,

    /**
     * A thread has finished dispatching.
     */
    THREAD_DISPATCH_COMPLETED,

    /**
     * A process has finished dispatching.
     */
    PROCESS_DISPATCH_COMPLETED,

    /**
     * A CPU burst has finished.
     */
    CPU_BURST_COMPLETED,

    /**
     * An IO burst has finished.
     */
    IO_BURST_COMPLETED,

    /**
     * Like CPU_BURST_COMPLETED, but also represents that the thread has
     * completely finished.
     */
    THREAD_COMPLETED,

    /**
     * A thread has been preempted before its time slice expired.
     */
    THREAD_PREEMPTED,

    /**
     * The OS needs to schedule a new thread to run, if one is available.
     */
    DISPATCHER_INVOKED
  };

  /**
   * The type of event.
   */
  Type type;

  /**
   * The time at which the event occurs.
   */
  int time;

  /**
   * The thread for which the event applies.
   */
  Thread* thread;

  /**
   * The associated scheduling decision, if any.
   */
  const SchedulingDecision* scheduling_decision;

  /**
   * Constructor.
   */
  Event(Type type, int time, Thread* thread)
      : Event(type, time, thread, nullptr) {}

  /**
   * Constructor.
   */
  Event(Type type, int time, Thread* thread, SchedulingDecision* sd)
      : type(type), time(time), thread(thread), scheduling_decision(sd) {}

  /**
   * Destructor.
   */
  ~Event() { delete scheduling_decision; }
};


/**
 * Comparator for std::priority_queue to correctly order event pointers.
 *
 * A binary predicate that takes two events as arguments and returns a bool.
 * The expression comp(a, b), where comp is an object of this type and a and b
 * are elements in the container, shall return true if a is considered to go
 * before b in the strict weak ordering the function defines.
 *
 * The priority queue will return the 'greatest' element, so larger times should
 * be considered 'smaller' in terms of priority.
 */
struct EventComparator {
  bool operator()(const Event* e1, const Event* e2) {
     return e1->time >= e2->time;
  }
};
