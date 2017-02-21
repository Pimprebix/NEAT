#include "InnovationBank.h"

InnovationBank* InnovationBank::_address = 0;
InnovationBank* InnovationBank::instance () {
   if (!_address) {
      _address = new InnovationBank();
   }
   return _address;
};

bool InnovationBank::isInnovationNew(int inputId, int outputId) {
    std::pair<int, int> aIdPair = std::make_pair(inputId, outputId);
    if (_innovationMap.find(aIdPair) == _innovationMap.end() )
    {
        return true;
    }
    else {
//        cerr << "innovation already exists!" << endl;
        return false;
    }
};