/**
 * This file contains implementations for methods in the PageTable class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "page_table/page_table.h"
#include <climits>

using namespace std;


size_t PageTable::get_present_page_count() const {
  size_t count = 0;

  for (size_t i = 0; i < rows.size(); i++){
    if (rows[i].present){
      count++;
    }
  }
  return count;
}


size_t PageTable::get_oldest_page() const {
  size_t loadTime = INT_MAX;
  size_t indexNumber = 0;

  for (size_t i = 0; i < rows.size(); i++) {
    const Row &row = rows[i];
    if (row.present) {
      if (row.loaded_at < loadTime) {
        indexNumber = i;
        loadTime = row.loaded_at;
      }
    }
  }
  return indexNumber;
}


size_t PageTable::get_least_recently_used_page() const {
  size_t accessTime = INT_MAX;
  size_t indexNumber = 0;

  for (size_t i = 0; i < rows.size(); i++) {
    const Row &row = rows[i];
    if (row.present) {
      if (row.last_accessed_at < accessTime) {
        indexNumber = i;
        accessTime = row.loaded_at;
      }
    }
  }
  return indexNumber;
}
