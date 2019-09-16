/**
 * This file contains implementations for methods in the VirtualAddress class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "virtual_address/virtual_address.h"
#include <bitset>
#include <sstream>


using namespace std;


VirtualAddress VirtualAddress::from_string(int process_id, string address) {
  string page_string = address.substr(0,10);
  string offset_string = address.substr(10,6);
  return VirtualAddress(process_id, bitset<16>(page_string).to_ulong(), bitset<16>(offset_string).to_ulong());
}


string VirtualAddress::to_string() const {
  bitset<16> resultString(page);
  resultString <<= OFFSET_BITS;
  resultString |= offset;
  return resultString.to_string();
}


ostream& operator <<(ostream& out, const VirtualAddress& address) {
  char result[60];
  sprintf(result, "PID %d @ %s [page: %lu; offset: %lu]", address.process_id, address.to_string().c_str(), 
          address.page, address.offset);
  return out << result;
}
