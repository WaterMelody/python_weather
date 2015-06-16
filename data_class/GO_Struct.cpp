#include "GO_Struct.h"

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

size_t GOTermSet::ParseGoa(const std::string& go_file) {
	ifstream fin(go_file);
  int node = 0;
	while (getline(fin, line)) {
		/*分割记录*/
		if (line != ""){
      if (line == "[Typedef]"){
        while (line != "")
          getline(fin,line);
      }
      if (line != "[Term]" && line != ""){
        std::vector<string> temp;
        boost::split( temp, line, boost::is_any_of( ":" ), boost::token_compress_on );
        switch (temp[0]) {
          case "id":
            node = atoi(temp[2].c_str());
            go_terms_[node].set_id(node);
            break;
          case "name":
            go_terms_[node].set_term(temp[1]);
          case "namespace":
            switch temp[1]{
              case " biological_process"
                go_terms_[node].set_type('P');
                break;
              case " molecular_function"
                go_terms_[node].set_type('F');
                break;
              case " cellular_component"
                go_terms_[node].set_type('C');
                break;
            }
            break;
          case "is_a":
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
		clog << "Total load " << go_terms_.size() << " protein sequences!" << endl;
	fin.close();
	return go_terms_.size();
}
