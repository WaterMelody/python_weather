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

AminoType GetAminoType(const std::string &x) {
	for	(int name = AminoType::NON; name <= AminoType::VAL; ++name)
		if (kTypeString[name] == x || (x.length() == 1 && x[0] == kTypeChar[name]))
			return static_cast<AminoType>(name);
	return AminoType::NON;
}

static const boost::property_tree::ptree& EmptyPtree() {
	static boost::property_tree::ptree t;
	return t;
}

std::string ProteinSequence::ToString() const {
	string ret_value("Protein sequence: \n");
	
	ret_value += "name: " + name() + "\n";
	ret_value += "species: " + species() + "\n";
	ret_value += "accessions:";
	for (auto &u : accessions())
		ret_value += " " + u;
	ret_value += "\n";
	ret_value += "GO: " + to_string(go_terms().size()) + "\n";
	for (auto &u : go_terms()) {
		ret_value += "GO_id: " + to_string(u.id_) + " ";
		ret_value += "GO_term: " + u.term_ + " ";
		ret_value += "GO_evidence: " + u.evidence_ + "\n";
	}
	ret_value += "ref pmids: " + to_string(ref_pmids().size()) + "\n";
	for (auto u : ref_pmids())
		ret_value += " " + to_string(u);
	ret_value += "\n";
	ret_value += "sequence: " + to_string(sequence().size()) + " ";
	for (auto u : sequence())
		ret_value += Get1LetterAminoName(u);
	ret_value += "\n";
	return ret_value;
}

size_t ProteinSequence::ParseUniprotPtree(const boost::property_tree::ptree& rt_tree) {
	using namespace boost::property_tree;
	
	if (rt_tree.empty())
		return 0;
	
	BOOST_FOREACH(const ptree::value_type& v, rt_tree) {
		if (v.first == "accession") {
			add_accession(v.second.data());
		}
		else if (v.first == "name") {
			vector<string> tokens;
			boost::split(tokens, v.second.data(), is_any_of("_"));
			if (tokens.size() != 2)
				cerr << "Warning: name tokens not equal to 2" << endl;
			else {
				set_name(tokens[0]);
				set_species(tokens[1]);
			}
		}
		else if (v.first == "reference") {
			const ptree &ptree_citation = v.second.get_child("citation", EmptyPtree());
			BOOST_FOREACH(const ptree::value_type& v2, ptree_citation) {
				if (v2.first == "dbReference") {
					string db = boost::to_upper_copy(v2.second.get("<xmlattr>.type", string()));
					if (db == "PUBMED") {
						int pmid = v2.second.get("<xmlattr>.id", -1);
						if (pmid < 0)
							cerr << "Warning: pmid not valid" << endl;
						else
							add_ref_pmid(pmid);
						if (pmid == 21248752) {
							cout << v2.first << endl;
							cout << v.first << endl;
						}
					}
				}
			}
		}
		else if (v.first == "dbReference") {
			string db = boost::to_upper_copy(v.second.get("<xmlattr>.type", string()));
			if (db == "GO") {
				string str_id = v.second.get("<xmlattr>.id", string());
				size_t pos = str_id.rfind(":") + 1;
				str_id = str_id.substr(pos);
				if (str_id.empty()) {
					cerr << "Warning: GO id is empty" << endl;
				}
				else {
					ProteinSequence::GOType go;
					BOOST_FOREACH(const ptree::value_type& v2, v.second) {
						string type = v2.second.get("<xmlattr>.type", string());
						if (type == "term")
							go.term_ = v2.second.get("<xmlattr>.value", string());
						else if (type == "evidence")
							go.evidence_ = v2.second.get("<xmlattr>.value", string());
					}
					go.id_ = atoi(str_id.data());
					add_go_term(go);
				}
			}
		}
		else if (v.first == "sequence") {
			string seq = v.second.data();
			for (auto u : seq) {
				AminoType amino = GetAminoType(string(1, u));
				if (amino != AminoType::NON)
					add_sequence_amino(amino);
			}
		}
	}
	SortGoTerm();
	return 1;
}

size_t ProteinSequence::ParseUniprotXml(const std::string& xml_file) {
	using namespace boost::property_tree;
	
	ptree rt_tree;
	try {
		read_xml(xml_file, rt_tree);
	} catch (const std::exception& e) {
		cerr << "Error: " << e.what() <<endl;
		return 0;
	}
	
	size_t success_cnt = 0;
	const ptree ptree_entry = rt_tree.get_child("uniprot.entry", EmptyPtree());
	if (ParseUniprotPtree(ptree_entry) > 0)
		++success_cnt;
	return success_cnt;
}

size_t ProteinSequenceSet::ParseUniprotXml(const std::string& xml_file) {
	using namespace boost::property_tree;
	
	const string kXmlHead = "<entry ";
	const string kXmlTail = "</entry>";
	
	ifstream fin(xml_file);
	string line;
	string xml_str;
	bool b_begin = false;
	size_t success_cnt = 0;
	while (getline(fin, line)) {
		string tmp_str = trim_left_copy(line);
		if (strncmp(tmp_str.c_str(), kXmlHead.c_str(), kXmlHead.size()) == 0)
			b_begin = true;
		if (b_begin)
			xml_str += line +  "\n";
		if (strncmp(tmp_str.c_str(), kXmlTail.c_str(), kXmlTail.size()) == 0) {
			istringstream sin(xml_str);
			
			ptree rt_tree;
			try {
				read_xml(sin, rt_tree);
			} catch (const std::exception& e) {
				cerr << "Error: " << e.what() <<endl;
			}
			ProteinSequence protein_sequence;
			size_t tmp_ret = protein_sequence.ParseUniprotPtree(rt_tree);
			if (tmp_ret > 0) {
				protein_sequences_[protein_sequence.name()] = protein_sequence;
				success_cnt += tmp_ret;
			}
			else 
				cerr << "Warning: Parse ptree error!, name = " << protein_sequence.name() << endl;
			
			if (log_status() == LogStatus::FULL_LOG && (success_cnt & 4095) == 0)
				clog << "\rLoaded " << success_cnt << " successfully";
			
			xml_str.clear();
			b_begin = false;
		}
	}
	clog << endl;
	fin.close();
	
	if (log_status() != SILENT)
		clog << "Total loaded " << success_cnt << " instances successfully" << endl;
	return success_cnt;
}

void ProteinSequenceSet::Save(const std::string& file_name) {
	ofstream fout(file_name);
	boost::archive::binary_oarchive oa(fout);
	oa << *this;
	fout.close();
}

size_t ProteinSequenceSet::Load(const std::string& file_name) {
	protein_sequences_.clear();
	
	ifstream fin(file_name);
	boost::archive::binary_iarchive ia(fin);
	ia >> *this;
	if (log_status() != LogStatus::SILENT)
		clog << "Total load " << protein_sequences_.size() << " protein sequences!" << endl;
	fin.close();
	return protein_sequences_.size();
}
