/**
 * This file contains implementations for methods in the Page class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "page/page.h"

using namespace std;


// Ensure PAGE_SIZE is initialized.
const size_t Page::PAGE_SIZE;


Page* Page::read_from_input(std::istream& in) {
  in >> std::noskipws;

  vector<char> bts;
  int count = 0;
  char c;

  while((count++ < 64) && (in >> c)) {
    bts.push_back(c);
  }

  if(bts.size()) {
    return new Page(bts);
  }
  
  return nullptr;
}


size_t Page::size() const {
  return bytes.size();
}


bool Page::is_valid_offset(size_t offset) const {
  return (offset < bytes.size());
}


char Page::get_byte_at_offset(size_t offset) {
  return bytes[offset];
}
