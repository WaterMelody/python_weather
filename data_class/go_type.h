#pragma once

#include <string>
#include <vector>
#include <unordered_map>

enum LogStatus {SILENT, PART_LOG, FULL_LOG};

class GOType {
public:
	std::string ToString() const;

	const int& id() const { return id_; }
	void set_id(const int& id) { id_ = id; }

	const std::string& term() const { return term_; }
	void set_name(const std::string& term) { term_ = term; }

	const char& type() const { return type_; }
	void set_type(const char& type) { type_ = type; }

	const std::vector<GOType>& fathers() const { return fathers_; }
	void set_fathers(const std::vector<GOType>& fathers) { fathers_ = fathers; SortFather()}

	const std::vector<GOType>& childs() const { return childs_; }
	void set_childs(const std::vector<GOType>& childs) { childs_ = childs; SortChild()}

	std::vector<int> FindAncestor(const int& node);

private:
	void add_child(const int child) { childs_.push_back(child); }
	void add_father(const int father) { fathers_.push_back(father); }

	void SortChild() {
		std::sort(childs_.begin(), childs_.end(), [](const int t1, const int t2) { return t1 < t2; });
	}
	void SortFather() {
		std::sort(fathers_.begin(), fathers_.end(), [](const int t1, const int t2) { return t1 < t2; });
	}

private:
	int id_;
	std::string term_;
	std::vector<int> fathers_
	std::vector<int> childs_;
	char type_
};

class GOTypeSet {
public:
	GOTypeSet(LogStatus log_status = PART_LOG): update_date_(0), log_status_(log_status) {}
	GOTypeSet(int update_date, LogStatus log_status = PART_LOG): update_date_(update_date), log_status_(log_status) {}


	std::size_t ParseGo(const std::string& go_file);

	void Save(const std::string& file_name) const;

	std::size_t Load(const std::string& file_name);

	int update_date() const { return update_date_; }
	void set_update_date(int update_date) { update_date_ = update_date; }

	std::vector<GOType>& gotypes() { return gotypes_; }

	LogStatus log_status() const { return log_status_; }
	void set_log_status(LogStatus log_status) { log_status_ = log_status; }

public:
	std::vector<GOType> gotypes_;


private:
	/*!@brief yyyymmdd*/
	int update_date_;
	LogStatus log_status_;
};
