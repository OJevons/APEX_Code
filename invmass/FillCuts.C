#include "FillCuts.h"


// FillCuts::FillCuts(TCutG* cutg, vector<pair<Double_t,Double_t>> cutgV)
// {

//   for(Int_t i = 0; i < cutgV.size(); i++){
//     cutg->SetPoint(i,cutgV[i].first,cutgV[i].second);
//   }
  

// }


void FillCuts::FillTCutG(TCutG* cutg, vector<pair<Double_t,Double_t>> cutgV)
{

  for(Int_t i = 0; i < cutgV.size(); i++){
    cutg->SetPoint(i,cutgV[i].first,cutgV[i].second);
  }

}
