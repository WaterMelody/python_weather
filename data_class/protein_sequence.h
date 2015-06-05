#pragma once

#include <string>
#include <vector>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

enum	AminoType {NON, ALA, ARG, ASN, ASP, CYS, GLU, GLN, GLY, HIS, ILE, LEU, LYS, MET, PHE, PRO, SER, THR, TRP, TYR, VAL};
const std::string kTypeString[] = { "NON", "ALA", "ARG", "ASN", "ASP", "CYS", "GLU", "GLN", "GLY", "HIS", "ILE", "LEU", "LYS", "MET", "PHE", "PRO", "SER", "THR", "TRP", "TYR", "VAL"};
const char kTypeChar[]          = { '0',    'A',   'R',   'N',   'D',   'C',   'E',   'Q',   'G',   'H',   'I',   'L',   'K',   'M',   'F',   'P',   'S',   'T',   'W',   'Y',   'V'  };

AminoType GetAminoType(const std::string &x);

inline std::string Get3LetterAminoName(AminoType type) {
	return kTypeString[type];
}

inline char Get1LetterAminoName(AminoType type) {
	return kTypeChar[type];
}

class ProteinSequence {
public:
	
	int ParsePtree(const boost::property_tree::ptree& tree);
	
	int ParseXml(const std::string& xml_file);
	
	const std::string& name() const { return name_; };
	void set_name(const string& name) { name_ = name; }
	
	const std::string& sequence() const { return sequence_; }
	void set_sequence(const std::string& sequence) { sequence_ = sequence; }
	
	/**
	 * @TODO complete other accessors and mutators
	 */
	
private:
	std::string name_;
	std::string spices_;
	std::vector<std::string> accessions_;
	std::vector<AminoType> sequence_;
	std::vector<int> go_ids_;
	std::vector<int> ref_pmids;
};
