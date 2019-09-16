/**
 * This file contains implementations for methods in the Process class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "process/process.h"

using namespace std;


Process* Process::read_from_input(std::istream& in) {
  // Define variable here
  vector<Page *> ps;
  Page *p = nullptr;
  size_t img_size = 0;

  while ((p = Page::read_from_input(in)) != nullptr){
    ps.push_back(p);
  }

  img_size += (ps.size() - 1) * Page::PAGE_SIZE + ps.back()->size();

  Process *result = new Process(img_size,ps);

  result->page_table = PageTable(ps.size());

  return result;
}


size_t Process::size() const {
  return num_bytes;
}


bool Process::is_valid_page(size_t index) const {
  return (index < pages.size());
}


size_t Process::get_rss() const {
  return this->page_table.get_present_page_count();
}


double Process::get_fault_percent() const {
  double result = 0.0;
  if(memory_accesses){
    result = 100.0 * page_faults / memory_accesses;
  }
  return result;
}
