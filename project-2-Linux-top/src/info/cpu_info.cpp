#include "cpu_info.h"
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


vector<CpuInfo> get_cpu_info() {
  // Create a cpu_info vector
  vector<CpuInfo> cpu_info;
  // Open /proc/stat file as cpu_info
  ifstream cpuinfo(PROC_ROOT "/stat");
  // Check whether file is open
  if(!cpuinfo){
    perror("Wrong in get_cpu_info");
    exit(1);
  }
  //int i = 0;
  string name;
  unsigned long long user,nice,sys,idle,io_wait,irq,softirq,steal,guest,guest_nice;
  while(!cpuinfo.eof()){
    CpuInfo cpu;
    cpuinfo >> name;
    if (name[0] == 'c' && name[1] == 'p' && name[2] == 'u'){
      cpuinfo >> user >> nice >> sys >> idle >> io_wait >> irq >> softirq >> steal >> guest >> guest_nice; 
      // cout << "cpu" << i << endl;
      // cout << "user_time: " << user << endl;
      // cout << "nice_time: " << nice << endl;
      // cout << "guest_nice_time: " << guest_nice << endl;
      cpu.user_time = user;
      cpu.nice_time = nice;
      cpu.system_time = sys;
      cpu.idle_time = idle;
      cpu.io_wait_time = io_wait;
      cpu.irq_time = irq;
      cpu.softirq_time = softirq;
      cpu.steal_time = steal;
      cpu.guest_time = guest;
      cpu.guest_nice_time = guest_nice;
      cpu_info.push_back(cpu);
      // i++;
    }
  }
  cpuinfo.close();
  return cpu_info;
}


CpuInfo operator -(const CpuInfo& lhs, const CpuInfo& rhs) {
  // TODO: implement me
  return CpuInfo();
}
