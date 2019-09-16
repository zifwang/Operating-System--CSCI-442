#include "system_info.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

using namespace std;


double get_uptime() {
  double up_time;
  // Open /proc/uptime file as cpu_info
  ifstream uptime(PROC_ROOT "/uptime");
  // Check whether file is open
  if(!uptime){
    perror("Wrong in get_uptime");
    exit(1);
  }
  uptime >> up_time;
  return up_time;
}


SystemInfo get_system_info() {
    // Create a SystemInfo struct
    SystemInfo sys_info;
    
    // get the number of processes currently existing on the system.
    sys_info.processes = get_all_processes(PROC_ROOT);
    sys_info.num_processes = sys_info.processes.size();
    // get num_user_threads and num_kernel_threads
    sys_info.num_user_threads = 0;
    sys_info.num_kernel_threads = 0;
    for (ProcessInfo a: sys_info.processes) {
        for (ProcessInfo b: a.threads) {
            // if is_user_thread()
            if (b.is_user_thread()) {
                sys_info.num_user_threads++;
            }
            // if is_kernel_thread()
            else if (b.is_kernel_thread()) {
                sys_info.num_kernel_threads++;
            }
        }
    }
    // get num_threads
    sys_info.num_threads = sys_info.num_user_threads + sys_info.num_kernel_threads;
    // get uptime
    sys_info.uptime = get_uptime();
    // get load_average
    sys_info.load_average = get_load_average();
    // get memory info
    sys_info.memory_info = get_memory_info();
    // get cpu info;
    sys_info.cpus = get_cpu_info();
    // get num_running
    string name;
    ifstream data(PROC_ROOT "/stat");
    if(!data){
        perror("Wrong in system_info");
        exit(1);
    }
    while(!data.eof()) {
        data >> name;
        if(name == "procs_running"){
            data >> sys_info.num_running;
        }
    }
    return sys_info;
}
