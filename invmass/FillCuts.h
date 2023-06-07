#ifndef FILLCUTS_H
#define FILLCUTS_H


#include "TCutG.h"

using namespace std;


class FillCuts { // Class used to fill TCutGs  
public: 
  FillCuts(){};
  void FillTCutG(TCutG* cutg, vector<pair<Double_t,Double_t>> cutgV);
private:
  
};


#endif
