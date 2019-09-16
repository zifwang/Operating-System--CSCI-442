//formatting
#include "formatting.h"
#include "../src/info/load_average_info.h"   // use info from load_average file
#include "../src/info/memory_info.h" 
#include "../src/info/process_info.h"
#include "../src/info/system_info.h"         // use info from system_info file
#include <math.h>
#include <string>
#include <stdio.h>
#include <unistd.h>              // SYSCONF
#include "statistics.h"
//#include <iostream>
using namespace std;

string format_uptime_info(double up_time){
    // double up_time = info.uptime;
    string uptime_format;
    int day = up_time/60/60/24; //A day = 60s*60min*24hours
    int hour = (int)(up_time/60/60)%24; //A hour  
    int minute = (int)(up_time/60)%60; //A min
    int second = (int)up_time%60; //A second
    string day_string = std::to_string(day);
    string hour_string;
    if (hour<10){
        hour_string = "0" + std::to_string(hour);
    }else{
        hour_string = std::to_string(hour);
    }
    string minute_string;
    if (minute<10){
        minute_string = "0" + std::to_string(minute);
    }else{
        minute_string = std::to_string(minute);
    }
    string second_string;
    if (second<10){
        second_string = "0" + std::to_string(second);
    }else{
        second_string = std::to_string(second);
    }
    //Convert seconds to days, hours::minutes::seconds format
    if(day == 0){
        uptime_format = "Up: "  + hour_string + ":" + minute_string + ":" + second_string;
    }else{
        uptime_format = "Up: " + day_string + " day, " + hour_string + ":" + minute_string + ":" + second_string;
    }

    return uptime_format;
}

string format_bytes(unsigned long long bytes){
    string bytes_format;
    unsigned long long unit_KB = 1024;
    unsigned long long unit_MB = unit_KB*1024;
    unsigned long long unit_GB = unit_MB*1024;
    // B case
    if(bytes < unit_KB){
        bytes_format = std::to_string(bytes) + " B";
    }
    // KiB case
    if(bytes < unit_MB && bytes > unit_KB){
        bytes = bytes/unit_KB;
        bytes_format = std::to_string(bytes) + " KiB";
    }
    // MiB case
    if(bytes < unit_GB && bytes > unit_MB){
        bytes = bytes/unit_MB;
        bytes_format = std::to_string(bytes) + " MiB";
    }
    // GiB case
    if(bytes > unit_GB){
        bytes = bytes/unit_GB;
        bytes_format = std::to_string(bytes) + " GiB";
    }

    return bytes_format;
}

string format_time(int second) {
	double second_mod = (double)second / 100;
	string time_format;
	int hour = second_mod / 3600;
	string hour_string;
	if (hour<10) {
		hour_string = "0" + std::to_string(hour);
	}
	else {
		hour_string = std::to_string(hour);
	}

	second_mod = second_mod - 3600 * hour;
	
	int mins = second_mod / 60;
	string minute_string;
	if (mins<10) {
		minute_string = "0" + std::to_string(mins);
	}
	else {
		minute_string = std::to_string(mins);
	}

	second_mod = second_mod - 60 * mins;

	// Final second
	double secs = second_mod;

	// sec format
	string second_string;
	char secs_string[8];

	// case where hour more than 100
	if (hour >= 10) {
		time_format = hour_string + "h";
	}
	//// case where hour between 100 to 0
	if (hour < 10 && hour > 0) {
		if (secs > 10) {
			int sec = secs * 100 / 100;
			second_string = std::to_string(sec);
		}
		else {
			int sec = secs * 100 / 100;
			second_string = "0" + std::to_string(sec);
		}
		hour_string = std::to_string(hour);
		time_format = hour_string + ":" + minute_string + ":" + second_string;
	}
	if (hour == 0) {
		minute_string = std::to_string(mins);
		sprintf(secs_string, "%4.2f", secs);
		second_string = secs_string;
		if (secs < 10) {
			second_string = "0" + second_string;
		}
		
		time_format = minute_string + ":" + second_string;
	}

	return time_format;
}

string format_load_averg_info(SystemInfo info){
    string la_format;
    double one = info.load_average.one_min;
    double five = info.load_average.five_mins;
    double fifteen = info.load_average.fifteen_mins;
    string one_string = std::to_string(one);
    string five_string = std::to_string(five);
    string fifteen_string = std::to_string(fifteen);

    // format load_average_info = one minutes + 5 mins + 15 mines
    la_format = "Load Average: " + one_string + ", " + five_string + ", " + fifteen_string;
    // printf("%s \n", la_format.c_str());
    return la_format;
}

string format_cpu_info(SystemInfo sys_new, SystemInfo sys_old, size_t cpu_num){
    string cpu_format;
    double user_perc = user_percent(sys_new, sys_old, cpu_num);
    double kernel_perc = kernel_percent(sys_new, sys_old, cpu_num);
    double idle_perc = idle_percent(sys_new, sys_old, cpu_num);
    string user_string = std::to_string(user_perc);
    string kernel_string = std::to_string(kernel_perc);
    string idle_string = std::to_string(idle_perc);
    // format cpu info  = user_perc + kernel_perc + idle_perc
    // printf("%d", cpu_num);
    if(cpu_num == 0){
        cpu_format = "CPUs: " + user_string + "% user, " + kernel_string + "% kernel, " + idle_string + "% idle";
    }else{
        cpu_format = "CPU" + std::to_string(cpu_num-1) + ": " + user_string + "% user, " + kernel_string + "% kernel, " + idle_string + "% idle";
    }
    return cpu_format;
}

string format_process_info(SystemInfo info){
    string process_format;
    unsigned process_num = info.num_processes;
    unsigned process_run = info.num_running;
    string num_string = std::to_string(process_num);
    string run_string = std::to_string(process_run);

    // format process info = total # of processes + running processes
    process_format = "Task: " + num_string + " total, " + run_string + " running";
    return process_format;
}

string format_threads_info(SystemInfo info){
    string threads_format;
    unsigned num_total = info.num_threads;
    string num_total_string = std::to_string(num_total);
    unsigned num_user = info.num_user_threads;
    string num_user_string = std::to_string(num_user);
    unsigned num_kernel = info.num_kernel_threads;
    string num_kernel_string = std::to_string(num_kernel);

    // format threads = total threads + user threads + kernel threads
    threads_format = "Threads: " + num_total_string + " total, " + num_user_string + " user, " + num_kernel_string + " kernel";
    return threads_format;
}

string format_memory_info(SystemInfo info){
    string memory_format;
    unsigned long long total = info.memory_info.total_memory;
    // use human_readable bytes function
    string installed = format_bytes(total);
    // free
    unsigned long long free_MEM = info.memory_info.free_memory;
    // use human_readable bytes function
    string free_string = format_bytes(free_MEM);
    // used memory = total - free - buffer -cached
    unsigned long long used = total - free_MEM - info.memory_info.buffers_memory - info.memory_info.cached_memory;
    string used_string = format_bytes(used);

    // format memory = installed + use + avaiable
    memory_format = "Memory: " + installed + " total, " + free_string + " free, " + used_string + " used";
    return memory_format;
}

char* format_table_header(){
    static char table_header[80];
    string pid = "PID";
    string res = "RES";
    string current_state = "S";
    string percent_cpu = "%CPU";
    string tm = "TIME";
    string command = "COMMAND";

    // contains pid, RES, s, %CPU, Time, Command 
    //table_header = pid + "    " + res + "    " + current_state + "    " + percent_cpu + "    " + tm + "    " + command;
    sprintf(table_header, "%-8s %-8s %-6s %-8s %-11s %s", pid.c_str(),res.c_str(),current_state.c_str(),percent_cpu.c_str(),tm.c_str(),command.c_str());
    return table_header;
}

char* format_table_entrees(ProcessInfo info){
    static char table_entrees[256];
    int pid = info.pid;
    string pid_string = std::to_string(pid);
    long rss = info.rss*sysconf(_SC_PAGESIZE);
    string rss_string = format_bytes(rss);
    char state = info.state;
    string state_string(1,state);
    double cpu_percent = info.cpu_percent;
    string cpu_string = std::to_string(cpu_percent);
    unsigned long tm = (info.utime + info.stime);
    string tm_string = format_time(tm);
    string command = info.command_line;

    // contains pid, RES, s, %CPU, Time, Command 
    // table_entrees = pid_string + "    " + rss_string + "    " + state_string + "    " + cpu_string + "    " + tm_string + "    " + command;
    sprintf(table_entrees, "%-8s %-8s %-6s %-8.2f %-11s %s", pid_string.c_str(),rss_string.c_str(),state_string.c_str(),cpu_percent,tm_string.c_str(),command.c_str());
    //sprintf(table_entrees, "%-5d %-7s %c %-5.1f %-8s %.220s", pid,rss_string,state,cpu_percent,tm_string,command);
    return table_entrees;
}
