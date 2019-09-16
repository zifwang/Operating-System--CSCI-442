/**
 * This file contains tests for the methods declared in memory_info.h.
 *
 * You must complete the implementations of the method stubs in memory_info.cpp
 * in whatever manner you choose, so long as these tests pass.
 */

#include "memory_info.h"
#include "gtest/gtest.h"

using namespace std;


TEST(GetMemoryInfo, TotalMemory) {
  MemoryInfo info = get_memory_info();
  EXPECT_EQ(8131884, info.total_memory);
}


TEST(GetMemoryInfo, FreeMemory) {
  MemoryInfo info = get_memory_info();
  EXPECT_EQ(5525628, info.free_memory);
}


TEST(GetMemoryInfo, BuffersMemory) {
  MemoryInfo info = get_memory_info();
  EXPECT_EQ(328328, info.buffers_memory);
}


TEST(GetMemoryInfo, CachedMemory) {
  MemoryInfo info = get_memory_info();
  EXPECT_EQ(1575808, info.cached_memory);
}


TEST(GetMemoryInfo, TotalSwap) {
  MemoryInfo info = get_memory_info();
  EXPECT_EQ(8342524, info.total_swap);
}


TEST(GetMemoryInfo, FreeSwap) {
  MemoryInfo info = get_memory_info();
  EXPECT_EQ(8241800, info.free_swap);
}
