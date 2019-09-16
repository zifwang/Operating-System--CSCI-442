//statistics
#pragma once
#include "../src/info/system_info.h"
#include "../src/info/process_info.h"

using namespace std;

/**
*  Calculate cpu info
*    -user_percent: user program use percentage of cpu 
*    -kernel_percent: kernel use percentage of cpu
*    -idle_percent: idle program use percentage of cpu
*    -cpu_utilization: each program use percentage of cpu
*/
// Calculate user_percent
double user_percent(SystemInfo sys_new, SystemInfo sys_old, size_t cpu_num);

// Calculate kernel_percent
double kernel_percent(SystemInfo sys_new, SystemInfo sys_old, size_t cpu_num);

// Calculate idle_percent
double idle_percent(SystemInfo sys_new, SystemInfo sys_old, size_t cpu_num);

// Calculate cpu_utilization.
void cpu_utilization(SystemInfo& sys_new, SystemInfo& sys_old);