#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>

enum LogStatus {SILENT, PART_LOG, FULL_LOG};

/**
 * @enumerator enum all amino types
 */
enum AminoType {NON, ALA, ARG, ASN, ASP, CYS, GLU, GLN, GLY, HIS, ILE, LEU, LYS, MET, PHE, PRO, SER, THR, TRP, TYR, VAL};
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
	struct GOType {
		int id_;
		std::string term_;
		std::string evidence_;
	};
	
public:
	std::string ToString() const;
	
	std::size_t ParsePtree(const boost::property_tree::ptree& ptree);
	
	std::size_t ParseXml(const std::string& xml_file);
	
	/**
	 * @brief accessors and mutators
	 */
	const std::string& name() const { return name_; }
	void set_name(const std::string& name) { name_ = boost::to_upper_copy(name); }
	
	const std::string& species() const { return species_; }
	void set_species(const std::string& species) { species_ = boost::to_upper_copy(species); }
	
	const std::vector<std::string>& accessions() const { return accessions_; }
	void set_accessions(const std::vector<std::string>& accessions) { 
		for (auto &u : accessions) 
			accessions_.push_back(boost::to_upper_copy(u));
	}
	void add_accession(const std::string& accession) { accessions_.push_back(accession); }
	
	const std::vector<AminoType>& sequence() const { return sequence_; }
	void set_sequence(const std::vector<AminoType>& sequence) { sequence_ = sequence; }
	void add_sequence_amino(AminoType amino) { sequence_.push_back(amino); }
	
	const std::vector<GOType>& go_terms() const { return go_terms_; }
	void set_go_terms(const std::vector<GOType>& go_terms) { go_terms_ = go_terms; }
	void add_go_term(GOType go_term) { go_terms_.push_back(go_term); }
	
	const std::vector<int>& ref_pmids() const { return ref_pmids_; }
	void set_ref_pmids(const std::vector<int>& ref_pmids) { ref_pmids_ = ref_pmids; }
	void add_ref_pmid(int pmid) { ref_pmids_.push_back(pmid); }
	
private:
	std::string name_;
	std::string species_;
	std::vector<std::string> accessions_;
	std::vector<AminoType> sequence_;
	std::vector<GOType> go_terms_;
	std::vector<int> ref_pmids_;
};

class ProteinSequenceSet {
public:
	std::size_t ParseXml(const std::string& xml_file);
	
	int update_date() const { return update_date_; }
	void set_update_date(int update_date) { update_date_ = update_date; }
	
	const std::unordered_map<std::string, ProteinSequence>& protein_sequences() const { return protein_sequences_; }
	
	LogStatus log_status() const { return log_status_; }
	void set_log_status(LogStatus log_status) { log_status_ = log_status; }
	
private:
	int update_date_;
	LogStatus log_status_;
	std::unordered_map<std::string, ProteinSequence> protein_sequences_;
	
};

