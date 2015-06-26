#include "go_term.h"

#include <iostream>
#include <cstdlib>
#include <exception>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <math.h>
#include <boost/algorithm/string.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

using namespace std;
using namespace boost;
size_t GOTermSet::ParseGo(const std::string& go_file) {
	ifstream fin(go_file);
	int node = 0;
	string line;
	std::map<std::string, int> x;
	x["id"]=1;
	x["name"]=2;
	x["namespace"]=3;
	x["is_a"]=4;
	x[" biological_process"]=5;
	x[" molecular_function"]=6;
	x[" cellular_component"]=7;
	while (getline(fin, line)) {
		/*分割记录*/
		if (line != ""){
      if (line == "[Typedef]"){
        while (line != "")
          getline(fin,line);
      }
      if (line != "[Term]" && line != ""){
        std::vector<string> temp;
        boost::split(temp, line, boost::is_any_of( ":" ), boost::token_compress_on );
        switch (x[temp[0]]) {
          case 1:
            node = atoi(temp[2].c_str());
            go_terms_[node].set_id(node);
            break;
          case 2:
            go_terms_[node].set_term(temp[1]);
          case 3:
            switch (x[temp[1]]){
              case 5:
                go_terms_[node].set_type('P');
                break;
              case 6:
                go_terms_[node].set_type('F');
                break;
              case 7:
                go_terms_[node].set_type('C');
                break;
        	  }
            break;
          case 4:
            std::vector<string> temp2;
            boost::split( temp2, temp[2], boost::is_any_of( " " ), boost::token_compress_on );
            int tmpchild = atoi(temp2[0].c_str());
            go_terms_[node].add_child(tmpchild);
            go_terms_[tmpchild].add_father(node);
            break;
         }
	  }
    }
  }
}

void GOTermSet::Save(const std::string& file_name) const {
	ofstream fout(file_name);
	boost::archive::binary_oarchive oa(fout);
	oa << *this;
	fout.close();
}

size_t GOTermSet::Load(const std::string& file_name) {
	go_terms_.clear();

	ifstream fin(file_name);
	boost::archive::binary_iarchive ia(fin);
	ia >> *this;
	if (log_status() != LogStatus::SILENT)
		clog << "Total load " << go_terms_.size() << " go terms!" << endl;
	fin.close();
	return go_terms_.size();
}