#include "memory_info.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


MemoryInfo get_memory_info() {
  // Open /meminfo file as mem_info
  ifstream mem_info(PROC_ROOT "/meminfo");
  // Check whether file is open
  if(!mem_info){
    perror("Wrong in get_memory_info");
    exit(1);
  }
  // Create a struct to return
  MemoryInfo info;
  // Three important variables
  string name;
  unsigned long long value;
  string unit;
  // loop through file
  while(!mem_info.eof()){
    mem_info >> name;
    //cout << name << endl;
    // Get total memory
    if(name == "MemTotal:"){
      mem_info >> value;
      info.total_memory = value;
    }
    // Get free memory
    if(name == "MemFree:"){
      mem_info >> value;
      info.free_memory = value;
    }
    // Get buffers memory
    if(name == "Buffers:"){
      mem_info >> value;
      info.buffers_memory = value;
    }
    // Get cached memory
    if(name == "Cached:"){
      mem_info >> value;
      info.cached_memory = value;
    }
    // Get total_swap memory
    if(name == "SwapTotal:"){
      mem_info >> value;
      info.total_swap = value;
    }
    // Get free_swap memory
    if(name == "SwapFree:"){
      mem_info >> value;
      info.free_swap = value;
    }
  }
  mem_info.close();
  return info;
}
