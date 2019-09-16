// sorting function used in sort function in Algorithm
//Code defining comparator functions
//   for ordering processes by PID, memory usage, CPU usage, and time running.
#include "../src/info/process_info.h"
#include "flags.h"

using namespace std;

bool my_sorting(const ProcessInfo& proc_new, const ProcessInfo& proc_old);
