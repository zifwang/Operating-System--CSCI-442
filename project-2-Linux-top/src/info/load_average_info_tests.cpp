/**
 * This file contains tests for the methods declared in load_average_info.h.
 *
 * You must complete the implementations of the method stubs in
 * load_average_info.cpp in whatever manner you choose, so long as these tests
 * pass.
 */

#include "load_average_info.h"
#include "gtest/gtest.h"

using namespace std;


TEST(GetLoadAverage, OneMinute) {
  EXPECT_DOUBLE_EQ(0.80, get_load_average().one_min);
}


TEST(GetLoadAverage, FiveMinute) {
  EXPECT_DOUBLE_EQ(0.29, get_load_average().five_mins);
}


TEST(GetLoadAverage, FifteenMinute) {
  EXPECT_DOUBLE_EQ(0.14, get_load_average().fifteen_mins);
}
