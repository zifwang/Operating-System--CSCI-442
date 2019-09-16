#include "../src/info/process_info.h"
#include "flags.h"

using namespace std;

bool my_sorting(const ProcessInfo& proc_new, const ProcessInfo& proc_old) {
    // if is CPU, order by the CPU used
    if (keys.sortKeys == CPU) {
        return (proc_new.cpu_percent > proc_old.cpu_percent);
    }

    // if is PID, order by the greatest pid
    if (keys.sortKeys == PID) {
        return (proc_new.pid < proc_old.pid);
    }

    // if is time, order by the time used
    if (keys.sortKeys == TIME) {
        return (proc_new.utime + proc_new.stime > proc_old.utime + proc_old.stime);
    }

    // if is MEM, order by the MEM used
    if (keys.sortKeys == MEM) {
        return (proc_new.rss > proc_old.rss);
    }

    return false;
}
