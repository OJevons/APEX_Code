#include "CsvParser.h"

CsvParser::CsvParser(string file_name):CsvName(file_name){

    ifstream infile(file_name);
    string line = "";
    
    //    vector<Column> Columns;
    
    
    int line_count = 0;
    while (getline(infile, line)){
        stringstream strstr(line);
        string word = "";
	vector<string> value_line; // values for one line
	int word_count = 0;
        while (getline(strstr,word, ',')){
	  if(line_count == 0){
	    vector<string> placeholder;
	    pair<string , vector<string>> head_col(word, placeholder);
	    Columns.push_back(head_col);
	  }
	  else{
	    Columns[word_count].second.push_back(word);
	  }
	  word_count++;
	}
	line_count++;
    }
    
    
  }


vector<string> CsvParser::GetColumn(int col){
  // get specified column
  
  vector<string> Col = Columns[col].second;
  return Col;

}

void CsvParser::SortByColumn(int col){
  // sort all columns by specified column

  vector<string> Col = Columns[col].second;
  vector<float> ColF;

  int N = Col.size();

  // create indice vector
  vector<int> Vind;
  
  for(int i = 0; i<N; i++){
    ColF.push_back(stod(Col[i]));
    Vind.push_back(i);
  }


  //  std::iota(Vind.begin(),Vind.end(),0);
  sort( Vind.begin(),Vind.end(), [&](int i,int j){return ColF[i]<ColF[j];} );


  // print re-arranged indices

  // cout << "about to rearrange" << endl;

  // for(int i = 0; i<N; i++){
  //   cout << "Vind[" << i << "] = " << Vind[i] << endl;  
  // }
  
  int count = 0;
  //  for(auto Col : Columns){
  int NCol = Columns.size();
  
  for(int j = 0; j<NCol; j++){
    vector<string> ColEnt = Columns[j].second; // original order
    
    for(int i = 0; i<N; i++){
      Columns[j].second.at(i) = ColEnt[Vind[i]];      
    }

    count++;
        
  }
  
  

  
  
}
