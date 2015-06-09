#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "data_class/protein_sequence.h"

using namespace std;

int main() {
	ofstream fout_tmp("tmp.txt");
	fout_tmp<< 2 << 3 << 4;
	fout_tmp.close();
	
  
	ProteinSequenceSet protein_set(20150100, LogStatus::FULL_LOG);
	size_t suc_cnt = 0;
	if ((suc_cnt = protein_set.ParseUniprotXml("../raw_data/uniprot_sprot_201501.xml")) > 0) {
	  cout << suc_cnt << " success" << endl;
	  protein_set.Save("uniprot_201501.protein_set");
	}
	else
		cerr << "Failed" << endl;
	
	//protein_set.Load("uniprot_201501.protein_set");
	ProteinSequence seq =  protein_set.protein_sequences().at("017L");
	std::vector<AminoType> ami = seq.sequence();
	for(size_t i = 0; i < ami.size(); i++) {
	  cout<<Get3LetterAminoName(ami[i]);
	}
	
	
	
	
	ofstream fout;
	fout.open("top100.txt");
	int cnt = 0;
	std::unordered_map<std::string, ProteinSequence> seq_top = protein_set.protein_sequences();
	for(auto it = seq_top.begin(); it != seq_top.end(); ++it){
	  ++cnt;
	  if(cnt > 100) break;
	  
	  cout<<it->first<<endl;
	  
	  fout<<it->first<<endl;
	  fout<<it->second.species()<<endl;
	  
	  
	  std::vector<AminoType> ami_top = it->second.sequence();
	  for(size_t i = 0; i < ami.size(); i++) {
	    fout<<Get3LetterAminoName(ami[i]);
	  }
	  fout<<endl;
	  
	  std::vector<ProteinSequence::GOType> go_top = it->second.go_terms();
	  for(std::vector<ProteinSequence::GOType>::iterator it2 = go_top.begin(); it2 != go_top.end(); ++it2){
	    fout<<it2->id_<<"	"<<(*it2).term_<<"	"<<(*it2).evidence_<<"	";
	  }
	  fout<<endl;
	}
	fout.close();
	
	
	
	clog << "Complete" << endl;
	return 0;
}