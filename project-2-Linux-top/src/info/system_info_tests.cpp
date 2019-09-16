/**
 * This file contains tests for the methods declared in system_info.h.
 *
 * You must complete the implementations of the method stubs in system_info.cpp
 * in whatever manner you choose, so long as these tests pass.
 */

#include "cpu_info.h"
#include "load_average_info.h"
#include "memory_info.h"
#include "process_info.h"
#include "system_info.h"
#include "gtest/gtest.h"

using namespace std;


TEST(GetSystemInfo, NumTasks) {
  SystemInfo sysinfo = get_system_info();
  EXPECT_EQ(220, sysinfo.num_processes);
}


TEST(GetSystemInfo, NumThreads) {
  SystemInfo sysinfo = get_system_info();
  EXPECT_EQ(279, sysinfo.num_threads);
}


TEST(GetSystemInfo, NumUserThreads) {
  SystemInfo sysinfo = get_system_info();
  EXPECT_EQ(148, sysinfo.num_user_threads);
}


TEST(GetSystemInfo, NumKernelThreads) {
  SystemInfo sysinfo = get_system_info();
  EXPECT_EQ(131, sysinfo.num_kernel_threads);
}


TEST(GetSystemInfo, NumRunning) {
  SystemInfo sysinfo = get_system_info();
  EXPECT_EQ(3, sysinfo.num_running);
}


TEST(GetSystemInfo, Uptime) {
  SystemInfo sysinfo = get_system_info();
  EXPECT_DOUBLE_EQ(361471.92, sysinfo.uptime);
}


TEST(GetSystemInfo, LoadAverageInfo) {
  SystemInfo sysinfo = get_system_info();
  LoadAverageInfo loadavg = get_load_average();

  EXPECT_DOUBLE_EQ(loadavg.one_min, sysinfo.load_average.one_min);
  EXPECT_DOUBLE_EQ(loadavg.five_mins, sysinfo.load_average.five_mins);
  EXPECT_DOUBLE_EQ(loadavg.fifteen_mins, sysinfo.load_average.fifteen_mins);
}


TEST(GetSystemInfo, MemoryInfo) {
  SystemInfo sysinfo = get_system_info();
  MemoryInfo memory_info = get_memory_info();

  EXPECT_EQ(memory_info.total_memory, sysinfo.memory_info.total_memory);
  EXPECT_EQ(memory_info.free_memory, sysinfo.memory_info.free_memory);
  EXPECT_EQ(memory_info.cached_memory, sysinfo.memory_info.cached_memory);
}


TEST(GetSystemInfo, CpuInfo) {
  SystemInfo sysinfo = get_system_info();
  vector<CpuInfo> cpus = get_cpu_info();

  ASSERT_EQ(cpus.size(), sysinfo.cpus.size());

  for (size_t i = 0; i < cpus.size(); i++) {
    EXPECT_EQ(cpus[i].idle_time, sysinfo.cpus[i].idle_time);
  }
}


TEST(GetSystemInfo, ProcessList) {
  SystemInfo sysinfo = get_system_info();
  vector<ProcessInfo> processes = get_all_processes(PROC_ROOT);

  EXPECT_EQ(processes.size(), sysinfo.processes.size());
}
