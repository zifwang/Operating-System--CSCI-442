#pragma once
#include "algorithms/scheduler.h"
#include "types/event.h"
#include "types/process.h"
#include "types/system_stats.h"
#include "util/logger.h"
#include <fstream>
#include <map>
#include <queue>


typedef std::priority_queue<const Event *, std::vector<const Event *>,
                            EventComparator>
    EventQueue;


class Simulation {
// PUBLIC API METHODS
public:

  Simulation(Scheduler* scheduler, Logger logger)
      : scheduler(scheduler), logger(logger) {}

  void run(const std::string& filename);

// EVENT HANDLING METHODS
private:

  void handle_thread_arrived(const Event* event);

  void handle_thread_dispatch_completed(const Event* event);

  void handle_process_dispatch_completed(const Event* event);

  void handle_cpu_burst_completed(const Event* event);

  void handle_io_burst_completed(const Event* event);

  void handle_thread_completed(const Event* event);

  void handle_thread_preempted(const Event* event);

  void handle_dispatcher_invoked(const Event* event);

// UTILITY METHODS
private:

  /**
   * Reads the given file and populates the initial event queue.
   */
  void read_file(const std::string& filename);

  /**
   * Reads a single process and its threads from the given input stream.
   */
  Process* read_process(std::istream& in);

  /**
   * Reads a single thread frmo the given input stream.
   */
  Thread* read_thread(std::istream& in, int tid, Process* process);

  /**
   * Calculates the overall statistics for the simulation.
   */
  SystemStats calculate_statistics();

  /**
   * Adds an event to the event queue.
   */
  void add_event(Event* event);

// CLASS INSTANCE VARIABLES
private:

  // You're not required to use all of these. As such, feel free to modify, add,
  // or delete.  :)

  /**
   * Map of PIDs to their corresponding processes.
   */
  std::map<int, Process*> processes;

  /**
   * The event queue containing all the events that still need to be processed.
   */
  EventQueue events;

  /**
   * The scheduler instance to use.
   */
  Scheduler* scheduler;

  /**
   * The logger to use.
   */
  Logger logger;

  /**
   * An object for storing various counters and metrics.
   */
  SystemStats stats;

  /**
   * The thread that is currently executing, or NULL.
   */
  Thread* active_thread = nullptr;

  /**
   * The thread that previously executed, or NULL.
   */
  Thread* prev_thread = nullptr;

  /**
   * The amount of overhead required to switch between two threads within the
   * same process.
   */
  size_t thread_switch_overhead;

  /**
   * The amount of overhead required to switch between two processes.
   */
  size_t process_switch_overhead;
};
