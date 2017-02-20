#include "InnovationBank.h"

InnovationBank* InnovationBank::_address = 0;
InnovationBank* InnovationBank::instance () {
   if (!_address) {
      _address = new InnovationBank();
   }
   return _address;
};