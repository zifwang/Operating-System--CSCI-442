#include "process_info.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <ctype.h>
#include <vector>



using namespace std;

bool is_number(const std::string& s){
  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isdigit(*it)) ++it;
  return !s.empty() && it == s.end();}



ProcessInfo get_process(int pid, const char* basedir) {
  // create ProcessInfo struct
  ProcessInfo info;
  // pid to string
  string pid_num = std::to_string(pid);
  // basedir to string
  string directory = std::string(basedir);
  /********************************************************************************************************/
  /*
  * Open /proc/[pid]/statm to get useful info.
  */
  string statm = "/statm";
  string statm_fn = directory + "/" + pid_num + statm;
  //cout << statm_fn << endl;
  std::ifstream statm_info(statm_fn.c_str());
  if(!statm_info){
    //perror("Wrong in statm_info");
    return ProcessInfo();
    // exit(1);
  }
  statm_info >> info.size >> info.resident >> info.share >> info.trs >> info.lrs >> info.drs >> info.dt;
  statm_info.close();
  /********************************************************************************************************/
  /********************************************************************************************************/
  /*
  * Open /proc/[pid]/stat to get useful info.
  */
  string stat = "/stat";
  string stat_fn = directory + "/" + pid_num + stat;
  // cout << stat_fn << endl; // test dir
  std::ifstream stat_info(stat_fn.c_str());
  if(!stat_info){
    //perror("Wrong in stat_info");
    return ProcessInfo();
    //exit(1);
  }
  stat_info >> info.pid >> info.comm >> info.state >> info.ppid >> info.pgrp >> info.session;
  stat_info >> info.tty_nr >> info.tpgid >> info.flags >> info.minflt >> info.cminflt >> info.majflt;
  stat_info >> info. cmajflt >> info.utime >> info.stime >> info.cutime >> info.cstime >> info.priority;
  stat_info >> info.nice >> info.num_threads >> info.itrealvalue >> info.starttime >> info.vsize >> info.rss;
  stat_info >> info.rsslim >> info.startcode >> info.endcode >> info.startstack >> info.kstkesp >> info.kstkeip;
  stat_info >> info.signal >> info.blocked >> info.sigignore >> info.sigcatch >> info.wchan >> info.nswap;
  stat_info >> info.cnswap >> info.exit_signal >> info.processor >> info.rt_priority >> info.policy >> info.delayacct_blkio_ticks;
  stat_info >> info.guest_time >> info.cguest_time;
  stat_info.close();
  /********************************************************************************************************/
  /********************************************************************************************************/
  /*
  * Open /proc/[pid]/cmdline to get useful info.
  */
  string cmdline = "/cmdline";
  string cmdline_fn = directory + "/" + pid_num + cmdline;
  std::ifstream cmdline_info(cmdline_fn.c_str());
  if(!cmdline_info){
    //perror("Wrong in cmdline_info");
    return ProcessInfo();
    //exit(1);
  }
  string cmd_line;
  cmdline_info >> cmd_line;
  for(size_t i = 0; i < cmd_line.length(); i++){
    char c = cmd_line[i];
    if(c == '\0'){
      info.command_line.push_back(' ');
    }
    else{
      info.command_line.push_back(c);
    }
  }
  if (!info.command_line.length()){
    info.command_line = string(info.comm + 1);
  }
  info.command_line.pop_back();
  cmdline_info.close();
  /********************************************************************************************************/
  /********************************************************************************************************/
  /*
  * Open /proc/[pid]/task to get useful info.
  */
  string task = "/task";
  string task_fn = directory + "/" + pid_num + task;
  // Convert string to pointer
  char *task_dir = new char[task_fn.size() + 1];
  std::copy(task_fn.begin(),task_fn.end(),task_dir);
  task_dir[task_fn.size()] = '\0'; // don't forget the terminating 0
  info.threads = get_all_processes(task_dir);
  for (size_t i = 0; i < info.threads.size(); i++){
    info.threads[i].tgid = pid;
  }
  //cout << info.threads.size() << endl;
  /********************************************************************************************************/
  return info;
  // return ProcessInfo();
}


vector<ProcessInfo> get_all_processes(const char* basedir) {
  // Create a list of info
  vector<ProcessInfo> info_list;
  DIR *dp;
  struct dirent *dirp;
  dp = opendir(basedir);
  if (dp == NULL) {
    return vector<ProcessInfo>();
  }
  while ((dirp = readdir(dp)) != NULL) {
    // cout << dirp->d_name << endl;
    string arg = dirp->d_name;
    // check whether string is digit
    if (is_number(arg)){
      // cout << "arg: " << arg << endl;
      int pid;
      pid = stoi(arg);
      // cout << "pid: " << pid << endl;
      info_list.push_back(get_process(pid, basedir));
    }
  }
  closedir(dp);
  return info_list;
  // return vector<ProcessInfo>();
}
