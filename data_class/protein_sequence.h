#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>

enum LogStatus {SILENT, PART_LOG, FULL_LOG};

enum SubtractProteinMode {MODIFIED_GO_TERM, ADD_NEW_GO_TERM, COMPLETE_NEW_ANNOTATION};

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

/*! @TODO complete the ParseGoa */
class ProteinSequence {
public:
	struct GOType {
		int id_;
		std::string term_;
		std::string evidence_;
		
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version) {
			ar & id_  & term_ & evidence_;
		}
	};
	
public:
	std::string ToString() const;
	
	/**
	 * @param ptree : uniprot database xml ptree structure, the ptree only contains a protein sequence
	 * @return 1 if load successfully, else return 0
	 */
	std::size_t ParseUniprotPtree(const boost::property_tree::ptree& ptree);
	
	/**
	 * @param xml_file : uniprot database xml file name, the xml file only contains a protein sequence
	 * @return 1 if load successfully, else return 0
	 */
	std::size_t ParseUniprotXml(const std::string& xml_file);
	
	/**
	 * @TODO complete the ParseGoa, can modify the interface if needed.
	 * @param goa_file : goa database file name, contains one instance
	 * @return the number of protein sequences loaded successfully
	 */
	std::size_t ParseGoa(const std::string& goa_file);
	
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
	void set_go_terms(const std::vector<GOType>& go_terms) { go_terms_ = go_terms; SortGoTerm(); }
	
	const std::vector<int>& ref_pmids() const { return ref_pmids_; }
	void set_ref_pmids(const std::vector<int>& ref_pmids) { ref_pmids_ = ref_pmids; }
	
private:
	friend boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & name_  & species_ & accessions_ & sequence_ & go_terms_ & ref_pmids_;
	}
	
	void add_go_term(const GOType& go_term) { go_terms_.push_back(go_term); }
	void add_ref_pmid(int pmid) { ref_pmids_.push_back(pmid); }
	
	void SortGoTerm() {
		std::sort(go_terms_.begin(), go_terms_.end(), [](const GOType& t1, const GOType& t2) { return t1.id_ < t2.id_; });
	}
	
private:
	std::string name_;
	std::string species_;
	std::vector<std::string> accessions_;
	std::vector<AminoType> sequence_;
	std::vector<GOType> go_terms_;
	std::vector<int> ref_pmids_;
};

/**
 * @TODO complete the ParseGoa 
 * @TODO complete Subtract
 */
class ProteinSequenceSet {
public:
	ProteinSequenceSet(LogStatus log_status = PART_LOG): update_date_(0), log_status_(log_status) {}
	ProteinSequenceSet(int update_date, LogStatus log_status = PART_LOG): update_date_(update_date), log_status_(log_status) {}
	
	/**
	 * @param xml_file : uniprot database xml file name, the xml file contains all protein sequence information
	 * @return the number of protein sequences loaded successfully
	 */
	std::size_t ParseUniprotXml(const std::string& xml_file);
	
	/**
	 * @TODO complete the ParseGoa, can modify the interface if needed.
	 * @param goa_file : goa database file name, contains all goa instances
	 * @return the number of protein sequences loaded successfully
	 */
	std::size_t ParseGoa(const std::string& goa_file);
	
	ProteinSequenceSet Subtract(const ProteinSequenceSet& protein_set, SubtractProteinMode mode) const;
	
	void Save(const std::string& file_name) const;
	
	/**
	 * @param file_name : protein_sequence_set serialization file name
	 * @return the number of protein sequence loaded successfully
	 */
	std::size_t Load(const std::string& file_name);
	
	int update_date() const { return update_date_; }
	void set_update_date(int update_date) { update_date_ = update_date; }
	
	const std::unordered_map<std::string, ProteinSequence>& protein_sequences() const { return protein_sequences_; }
	
	LogStatus log_status() const { return log_status_; }
	void set_log_status(LogStatus log_status) { log_status_ = log_status; }
	
private:
	friend boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & update_date_  & log_status_ & protein_sequences_;
	}
	
private:
	/*!@brief yyyymmdd*/
	int update_date_;
	LogStatus log_status_;
	std::unordered_map<std::string, ProteinSequence> protein_sequences_;
	
};

