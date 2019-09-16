#include "load_average_info.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


LoadAverageInfo get_load_average() {
  // Open /proc/loadavg file as info
  ifstream info(PROC_ROOT "/loadavg");
  // Check whether file is open or not
  if(!info){
    perror("Wrong in get_load_average");
    exit(1);
  }
  LoadAverageInfo load_average_info;
  double one_min, five_mins, fifteen_mins, value_1, value_2;
  info >> one_min >> five_mins >> fifteen_mins >> value_1 >> value_2;
  // load info to LoadAverageInfo Struct
  // Debug use
  // cout << one_min << endl;
  // cout << five_mins << endl;
  // cout << fifteen_mins << endl;
  load_average_info.one_min = one_min;
  load_average_info.five_mins = five_mins;
  load_average_info.fifteen_mins = fifteen_mins;    
  info.close();
  return load_average_info;
}
