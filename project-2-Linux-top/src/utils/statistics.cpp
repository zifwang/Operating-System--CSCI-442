//statistics
#include "../src/utils/statistics.h"
#include "../src/info/system_info.h"
#include "../src/info/process_info.h"

using namespace std;

double user_percent(SystemInfo sys_new, SystemInfo sys_old, size_t cpu_num){
    
    double user_time_diff = sys_new.cpus[cpu_num].user_time - sys_old.cpus[cpu_num].user_time;
    double total_time_diff =  (double)(sys_new.cpus[cpu_num].total_time() - sys_old.cpus[cpu_num].total_time());
    double value = user_time_diff/total_time_diff * 100;

    return value;
}

double kernel_percent(SystemInfo sys_new, SystemInfo sys_old, size_t cpu_num){

    double total_sys_time_diff = sys_new.cpus[cpu_num].total_system_time() - sys_old.cpus[cpu_num].total_system_time();
    double total_time_diff = (double)(sys_new.cpus[cpu_num].total_time() - sys_old.cpus[cpu_num].total_time());
    double value = total_sys_time_diff/total_time_diff * 100;

    return value;
}

double idle_percent(SystemInfo sys_new, SystemInfo sys_old, size_t cpu_num){

    double idle_time_diff = sys_new.cpus[cpu_num].total_idle_time() - sys_old.cpus[cpu_num].total_idle_time();
    double total_time_diff = (double)(sys_new.cpus[cpu_num].total_time() - sys_old.cpus[cpu_num].total_time());
    double value = idle_time_diff/total_time_diff * 100;

    return value;
}

void cpu_utilization(SystemInfo& sys_new, SystemInfo& sys_old){
    // Create a map to store old process info
    std::unordered_map<int, ProcessInfo *> old_process;

    // Loop through sys_old->processes and put in the map
    for (ProcessInfo& process: sys_old.processes){
        old_process[process.pid] = &process;
    }
    
    // Loop through sys_new->processes
    for (ProcessInfo& process: sys_new.processes) {
        // Find correct pid
        if (old_process.count(process.pid)) {
            // new process utime + stime
            double new_tm = process.utime + process.stime;
            // old process utime + stime with the same pid to new process
            double old_tm = old_process[process.pid]->utime + old_process[process.pid]->stime;
            double delta_tm = new_tm - old_tm;
            double delta_total_tm = sys_new.cpus[1].total_time() - sys_old.cpus[1].total_time();
            process.cpu_percent = (double)delta_tm/delta_total_tm*100;
        }
        // Not, set to 0
        else process.cpu_percent = 0;
    }
}