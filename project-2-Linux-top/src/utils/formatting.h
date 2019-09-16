// formatting
// Code to help with formatting numbers in
// readable formats, e.g. human-readably bytes (B, KiB, MiB, GiB, etc) and
// time strings (seconds, minutes, hours, days, etc).
#pragma once
#include "../src/info/load_average_info.h"   // use info from load_average file
#include "../src/info/system_info.h"         // use info from system_info file
#include "../src/info/process_info.h"        // use info from process_info.h
#include <string>
using namespace std;

    /**
     * System uptime: how long the computer has been on, with appropriate labeling.
     * For example: XXdays, HH:MM:ss
     */
    string format_uptime_info(double up_time);

    /** 
    * human_readable bytes convertion 
    */
    string format_bytes(unsigned long long bytes);

    /**
    * human readable time convertion
    */
    string format_time(int second);

    /**
     * Load average over the last 1, 5, and 15 minutes
     */
    string format_load_averg_info(SystemInfo info);

    /**
     * Percent of time 1) all processors and 2) each individual processor spent:
     *   – in user mode
     *   – in kernel mode
     *   – idling
    */
    string format_cpu_info(SystemInfo sys_new, SystemInfo sys_old, size_t cpu_num);

    /**
     * Total number of processes that currently exist on the system
     * Total number of processes that are currently running
     */
    string format_process_info(SystemInfo info);

    /**
     * Total number of threads (both user-land and kernel-level)
     */
    string format_threads_info(SystemInfo info);

    /**
     * The amount of memory:
     *   – that is installed on the machine
     *   – that is currently in use
     *   – that is currently available
     */
    string format_memory_info(SystemInfo info);

    /**
     * Table display
     * Display a process table with the following (labeled) columns:
     *       • Process ID (PID)
     *       • Resident memory size
     *       • Current state (single-letter abbreviation)
     *       • % of CPU currently being used
     *       • Total amount of time spent being executed by a processor (HH:MM:SS)
     *       • The name of the binary (or the cmdline) that was executed
     */ 
    char* format_table_header();
    char* format_table_entrees(ProcessInfo info);

    