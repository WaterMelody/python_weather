#include "protein_sequence.h"

#include <iostream>
#include <cstdlib>
#include <exception>
#include <string>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

using namespace std;
using namespace boost;

size_t ProteinSequenceSet::ParseGOA(const std::string& goa_file) {
	ifstream fin(goa_file);
	string line;
	string go;
	string name;
	string name_last;
	std::vector<GOType> go_terms;
	while (getline(fin, line)) {
		string tmp_str = line;
		GOType tempgo;
		/*分割记录*/
		vector<string> vStr;
		boost::split( vStr, tmp_str, boost::is_any_of( "	" ), boost::token_compress_on );
		/*提取出符合 Uniprot 格式的名称*/
		vector<string> tmpname;
		boost::split( tmpname, vStr[9], boost::is_any_of( "|" ), boost::token_compress_on );
		name = tmpname[0];
		if (name!=name_last){
			/*当该条记录的蛋白质与上条不同时，导出上条数据并清空暂存GO序列*/
			ProteinSequence protein_sequence;
			protein_sequence.set_go_terms(go_terms);
			vector<string> tmpname2;
			boost::split( tmpname2, name_last, boost::is_any_of( "_" ), boost::token_compress_on );
			protein_sequence.set_name(tmpname2[0]);
			protein_sequence.set_species(tmpname2[1]);
			protein_sequences_.push_back(protein_sequence);
			std::vector<GOType> go_terms;
		}
		/*导入 GO 记录至暂存序列中*/
		tempgo.evidence_ = vStr[5];
		go = vStr[3];
		tempgo.id_ = atoi(go);
		tempgo.term_ = vStr[7]+':'+vStr[8];
		go_terms.push_back(tempgo);
		name_last = name;
	}
	/*最后一行的处理*/
	ProteinSequence protein_sequence;
	protein_sequence.set_go_terms(go_terms);
	vector<string> tmpname2;
	boost::split( tmpname2, name_last, boost::is_any_of( "_" ), boost::token_compress_on );
	protein_sequence.set_name(tmpname2[0]);
	protein_sequence.set_species(tmpname2[1]);
	protein_sequences_.push_back(protein_sequence);
	std::vector<GOType> go_terms;
	clog << endl;
	fin.close();
}
