#pragma once


/**
 * Encapsulates various system statistics.
 */
struct SystemStats {

  /**
   * The total amount of time that has elapsed in the simulation.
   */
  size_t total_time = 0;

  /**
   * The amount of time that the processor has been idle.
   */
  size_t total_idle_time = 0;

  /**
   * The amount of time that the processor has spent dispatching (overhead).
   */
  size_t dispatch_time = 0;

  /**
   * The amount of time that the processor has spent executing threads.
   */
  size_t service_time = 0;

  /**
   * The cumulative amount of time that threads spent doing I/O.
   */
  size_t io_time = 0;

  /**
   * The amount of time that the processor was in use (service time + dispatch
   * time).
   */
  size_t total_cpu_time = 0;

  /**
   * The percentage of time during which the CPU was utilized (service time +
   * dispatch time).
   */
  double cpu_utilization = 0.0;

  /**
   * The percentage of time spent doing actual user work (service time).
   */
  double cpu_efficiency = 0.0;

  /**
   * The count of threads of different priorities.
   */
  size_t thread_counts[4] = {0, 0, 0, 0};

  /**
   * The average response times for threads of different priorities.
   */
  double avg_thread_response_times[4] = {0.0, 0.0, 0.0, 0.0};

  /**
   * The average turnaround times for threads of different priorities.
   */
  double avg_thread_turnaround_times[4] = {0.0, 0.0, 0.0, 0.0};
};
