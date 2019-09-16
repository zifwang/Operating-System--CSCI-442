/**
 * This file contains tests for the methods declared in cpu_info.h.
 *
 * You must complete the implementations of the method stubs in cpu_info.cpp in
 * whatever manner you choose, so long as these tests pass.
 */

#include "cpu_info.h"
#include "gtest/gtest.h"

using namespace std;


TEST(GetCpuInfo, UserTime) {
  vector<CpuInfo> cpus = get_cpu_info();
  ASSERT_EQ(9, cpus.size());

  EXPECT_EQ(713573LL, cpus[0].user_time);
  EXPECT_EQ(110229LL, cpus[1].user_time);
  EXPECT_EQ(44418LL, cpus[5].user_time);
}


TEST(GetCpuInfo, NiceTime) {
  vector<CpuInfo> cpus = get_cpu_info();
  ASSERT_EQ(9, cpus.size());

  EXPECT_EQ(182653LL, cpus[0].nice_time);
  EXPECT_EQ(1281LL, cpus[1].nice_time);
  EXPECT_EQ(8761LL, cpus[5].nice_time);
}


TEST(GetCpuInfo, SystemTime) {
  vector<CpuInfo> cpus = get_cpu_info();
  ASSERT_EQ(9, cpus.size());

  EXPECT_EQ(215417LL, cpus[0].system_time);
  EXPECT_EQ(37212LL, cpus[1].system_time);
  EXPECT_EQ(16826LL, cpus[5].system_time);
}


TEST(GetCpuInfo, IdleTime) {
  vector<CpuInfo> cpus = get_cpu_info();
  ASSERT_EQ(9, cpus.size());

  EXPECT_EQ(287474103LL, cpus[0].idle_time);
  EXPECT_EQ(35954089LL, cpus[1].idle_time);
  EXPECT_EQ(36069751LL, cpus[5].idle_time);
}


TEST(GetCpuInfo, IoWaitTime) {
  vector<CpuInfo> cpus = get_cpu_info();
  ASSERT_EQ(9, cpus.size());

  EXPECT_EQ(145673LL, cpus[0].io_wait_time);
  EXPECT_EQ(17078LL, cpus[1].io_wait_time);
  EXPECT_EQ(2466LL, cpus[5].io_wait_time);
}


TEST(GetCpuInfo, IrqTime) {
  vector<CpuInfo> cpus = get_cpu_info();
  ASSERT_EQ(9, cpus.size());

  EXPECT_EQ(2LL, cpus[0].irq_time);
  EXPECT_EQ(0LL, cpus[1].irq_time);
  EXPECT_EQ(0LL, cpus[5].irq_time);
}


TEST(GetCpuInfo, SoftIrqTime) {
  vector<CpuInfo> cpus = get_cpu_info();
  ASSERT_EQ(9, cpus.size());

  EXPECT_EQ(51404LL, cpus[0].softirq_time);
  EXPECT_EQ(214LL, cpus[1].softirq_time);
  EXPECT_EQ(0LL, cpus[5].softirq_time);
}


TEST(GetCpuInfo, StealTime) {
  vector<CpuInfo> cpus = get_cpu_info();
  ASSERT_EQ(9, cpus.size());

  EXPECT_EQ(0LL, cpus[0].steal_time);
  EXPECT_EQ(0LL, cpus[1].steal_time);
  EXPECT_EQ(0LL, cpus[5].steal_time);
}


TEST(GetCpuInfo, GuestTime) {
  vector<CpuInfo> cpus = get_cpu_info();
  ASSERT_EQ(9, cpus.size());

  EXPECT_EQ(0LL, cpus[0].guest_time);
  EXPECT_EQ(0LL, cpus[1].guest_time);
  EXPECT_EQ(0LL, cpus[5].guest_time);
}


TEST(GetCpuInfo, GuestNiceTime) {
  vector<CpuInfo> cpus = get_cpu_info();
  ASSERT_EQ(9, cpus.size());

  EXPECT_EQ(0LL, cpus[0].guest_nice_time);
  EXPECT_EQ(0LL, cpus[1].guest_nice_time);
  EXPECT_EQ(0LL, cpus[5].guest_nice_time);
}
