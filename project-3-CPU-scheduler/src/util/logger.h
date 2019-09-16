#pragma once
#include <string>
#include "types/event.h"
#include "types/process.h"
#include "types/thread.h"
#include "types/scheduling_decision.h"
#include "types/system_stats.h"


enum Color {
  GREEN,
  GRAY,
  LIGHT_GRAY
};


class Logger {
public:

  Logger(bool verbose, bool per_thread)
      : verbose(verbose), per_thread(per_thread) {}

  /**
   * If 'verbose' is set to true, outputs a human-readable message indicating
   * that the thread associated with the given event has transitioned from
   * before_state to after_state.
   */
  void print_state_transition(
      const Event* event,
      Thread::State before_state,
      Thread::State after_state) const;

  /**
   * Outputs a message if 'verbose' is true.
   */
  void print_verbose(
      const Event* event,
      Thread* thread,
      std::string message) const;

  /**
   * If 'per_thread' is set to true, outputs detailed information about a
   * process and its threads.
   */
  void print_process_details(Process* process) const;

  /**
   * Print overall statistics for the simulation.
   */
  void print_statistics(SystemStats stats) const;

private:

  /**
   * Whether to display verbose output.
   */
  bool verbose;

  /**
   * Whether to display detailed per-process statistics.
   */
  bool per_thread;

  /**
   * Formats the given text as using the given ANSI color code.
   */
  template <typename T>
  std::string colorize(Color color, T text) const;

  /**
   * Formats the given text as using the given ANSI color code, after first
   * substituting it into the given format_str.
   */
  template <typename T>
  std::string colorize(Color color, std::string format_str, T text) const;
};
