/**
 * This file contains tests for the methods declared in process_info.h.
 *
 * You must complete the implementations of the method stubs in process_info.cpp
 * in whatever manner you choose, so long as these tests pass.
 */

#include "process_info.h"
#include "gtest/gtest.h"
#include <algorithm>

using namespace std;


TEST(GetProcess, MemorySize) {
  ProcessInfo process = get_process(16224, PROC_ROOT);
  EXPECT_EQ(1805, process.size);
}


TEST(GetProcess, ResidentSize) {
  ProcessInfo process = get_process(16224, PROC_ROOT);
  EXPECT_EQ(154, process.resident);
}


TEST(GetProcess, SharedSize) {
  ProcessInfo process = get_process(16224, PROC_ROOT);
  EXPECT_EQ(130, process.share);
}


TEST(GetProcess, ProcessId) {
  ProcessInfo process = get_process(16224, PROC_ROOT);
  EXPECT_EQ(16224, process.pid);
}


TEST(GetProcess, ProcessFlags) {
  ProcessInfo process = get_process(16224, PROC_ROOT);
  EXPECT_EQ(4218880, process.flags);
}


TEST(GetProcess, UserTime) {
  ProcessInfo process = get_process(16224, PROC_ROOT);
  EXPECT_EQ(1010, process.utime);
}


TEST(GetProcess, SystemTime) {
  ProcessInfo process = get_process(16224, PROC_ROOT);
  EXPECT_EQ(1010, process.utime);
}


TEST(GetProcess, ChildUserTime) {
  ProcessInfo process = get_process(1656, PROC_ROOT);
  EXPECT_EQ(484229, process.cutime);
}


TEST(GetProcess, ChildSystemTime) {
  ProcessInfo process = get_process(1656, PROC_ROOT);
  EXPECT_EQ(143600, process.cstime);
}


TEST(GetProcess, RssLim) {
  ProcessInfo process = get_process(16224, PROC_ROOT);
  EXPECT_EQ(18446744073709551615UL, process.rsslim);
}


TEST(GetProcess, SigCatch) {
  ProcessInfo process = get_process(16224, PROC_ROOT);
  EXPECT_EQ(514, process.sigcatch);
}


TEST(GetProcess, Command) {
  ProcessInfo process = get_process(16224, PROC_ROOT);
  EXPECT_STRCASEEQ("(dd)", process.comm);
}


TEST(GetProcess, CommandLine) {
  ProcessInfo process = get_process(16224, PROC_ROOT);
  string expected = "dd if=/dev/zero of=/dev/null";

  EXPECT_EQ(expected, process.command_line);
}


TEST(GetProcess, CommandLineDefaultsToCommandWhenEmpty) {
  ProcessInfo process = get_process(10, PROC_ROOT);
  string expected = "rcuos/2";

  EXPECT_EQ(expected, process.command_line);
}


TEST(GetProcess, Threads) {
  ProcessInfo process = get_process(1719, PROC_ROOT);

  ASSERT_EQ(13, process.threads.size());

  EXPECT_EQ(1719, process.threads[0].tgid);
  EXPECT_EQ(1719, process.threads[11].tgid);
}


TEST(GetProcess, WithNestedBasePath) {
  ProcessInfo process = get_process(1735, PROC_ROOT "/1719/task");

  EXPECT_EQ(1735, process.pid);
}


TEST(GetProcess, WithNestedBasePathThreads) {
  ProcessInfo process = get_process(1735, PROC_ROOT "/1719/task");

  EXPECT_EQ(0, process.threads.size());
}


// Simple comparator function for ordering processes by their PIDs.
bool process_comparator(const ProcessInfo& p1, const ProcessInfo& p2) {
  return p1.pid < p2.pid;
}


TEST(GetAllProcesses, CorrectProcessContents) {
  vector<ProcessInfo> processes = get_all_processes(PROC_ROOT);
  ASSERT_EQ(220, processes.size());

  sort(processes.begin(), processes.end(), process_comparator);

  EXPECT_STRCASEEQ("(init)", processes[0].comm);
  EXPECT_STRCASEEQ("(kthreadd)", processes[1].comm);
  EXPECT_STRCASEEQ("(sshd)", processes[133].comm);
  EXPECT_STRCASEEQ("(accounts-daemon)", processes[140].comm);
}


TEST(GetAllProcesses, SubdirectoryCount) {
  vector<ProcessInfo> processes = get_all_processes(PROC_ROOT "/1719/task");
  ASSERT_EQ(13, processes.size());

  sort(processes.begin(), processes.end(), process_comparator);

  EXPECT_EQ(1719, processes[0].pid);
  EXPECT_EQ(1731, processes[1].pid);
  EXPECT_EQ(1734, processes[4].pid);
  EXPECT_EQ(16822, processes[12].pid);
}
