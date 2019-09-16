/**
 * This file contains implementations for methods in the PhysicalAddress class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "physical_address/physical_address.h"
#include <bitset>

using namespace std;


string PhysicalAddress::to_string() const {
    bitset<16> resultString(frame);
    resultString <<= OFFSET_BITS;
    resultString |= offset;
  return resultString.to_string();
}


ostream& operator <<(ostream& out, const PhysicalAddress& address) {
  return out << address.to_string() << " [frame: " << address.frame << "; offset: " << address.offset << "]";
}
