#include "logger.h"
#include "types/thread.h"
#include "types/event.h"
#include <boost/format.hpp>
#include <cstdio>
#include <sstream>
#include <iostream>

using namespace std;
using boost::format;


const char* COLOR_MAP[3] = {
  "\033[1;32m",
  "\033[1;30m",
  "\033[1;37m"
};


const char* STATE_MAP[5] = {
  "NEW",
  "READY",
  "RUNNING",
  "BLOCKED",
  "EXIT"
};


const char* PROCESS_TYPE_MAP[4] = {
  "SYSTEM",
  "INTERACTIVE",
  "NORMAL",
  "BATCH"
};


const char* EVENT_MAP[8] = {
  "THREAD_ARRIVED",
  "THREAD_DISPATCH_COMPLETED",
  "PROCESS_DISPATCH_COMPLETED",
  "CPU_BURST_COMPLETED",
  "IO_BURST_COMPLETED",
  "THREAD_COMPLETED",
  "THREAD_PREEMPTED",
  "DISPATCHER_INVOKED"
};


void Logger::print_verbose(
    const Event* event,
    Thread* thread,
    string message) const {
  if (!verbose){
    return;
  }

  format fmt(
      "At time %d:\n"
      "    %s\n"
      "    Thread %d in process %d %s\n"
      "    %s\n\n");

  Color color = (event->type == Event::DISPATCHER_INVOKED) ? LIGHT_GRAY : GREEN;

  cout << fmt
      % event->time
      % colorize(color, EVENT_MAP[event->type])
      % thread->id
      % thread->process->pid
      % colorize(GRAY, "[%s]", PROCESS_TYPE_MAP[thread->process->type])
      % message;
}


void Logger::print_state_transition(
    const Event* event,
    Thread::State before_state,
    Thread::State after_state) const {
  format message("Transitioned from %s to %s");

  message
      % STATE_MAP[before_state]
      % STATE_MAP[after_state];

  print_verbose(event, event->thread, message.str());
}


void Logger::print_process_details(Process* process) const {
  if (!per_thread) {
    return;
  }

  cout << format("%s %s:\n")
      % colorize(LIGHT_GRAY, "Process %d", process->pid)
      % colorize(GRAY, "[%s]", PROCESS_TYPE_MAP[process->type]);

  format thread_format(
    "    Thread %d:  ARR: %-6lu CPU: %-6lu I/O: %-6lu TRT: %-6lu END: %-6lu\n");

  for (size_t i = 0; i < process->threads.size(); i++) {
    Thread* thread = process->threads[i];

    cout << thread_format
        % thread->id
        % thread->arrival_time
        % thread->service_time
        % thread->io_time
        % thread->turnaround_time()
        % thread->end_time;
  }

  cout << "\n";
}


void Logger::print_statistics(SystemStats stats) const {
  format process_type_fmt(
      "%s\n"
      "    %-20s %8lu\n"
      "    %-20s %8.2lf\n"
      "    %-20s %8.2lf\n\n");

  format summary_fmt(
      "%-20s %12lu\n"
      "%-20s %12lu\n"
      "%-20s %12lu\n"
      "%-20s %12lu\n"
      "%-20s %12lu\n"
      "\n"
      "%-20s %11.2lf%%\n"
      "%-20s %11.2lf%%\n");

  cout << colorize(GREEN, "SIMULATION COMPLETED!\n\n");

  for (int i = Process::SYSTEM; i <= Process::BATCH; i++) {
    cout << process_type_fmt
        % colorize(GRAY, "%s THREADS:", PROCESS_TYPE_MAP[i])
        % "Total count:" % stats.thread_counts[i]
        % "Avg response time:" % stats.avg_thread_response_times[i]
        % "Avg turnaround time:" % stats.avg_thread_turnaround_times[i];
  }

  cout << summary_fmt
      % "Total elapsed time:" % stats.total_time
      % "Total service time:" % stats.service_time
      % "Total I/O time:" % stats.io_time
      % "Total dispatch time:" % stats.dispatch_time
      % "Total idle time:" % stats.total_idle_time
      % "CPU utilization:" % stats.cpu_utilization
      % "CPU efficiency:" % stats.cpu_efficiency;

  cout << endl;
}


template<typename T>
string Logger::colorize(Color color, T text) const {
  return (format("%s%s%s") % COLOR_MAP[color] % text % "\033[0m").str();
}


template<typename T>
string Logger::colorize(Color color, string format_str, T text) const {
  return colorize(color, (format(format_str) % text).str());
}
