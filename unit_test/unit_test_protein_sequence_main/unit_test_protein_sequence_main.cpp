#include <iostream>
#include <string>
#include <vector>

#include "data_class/protein_sequence.h"

using namespace std;

int main() {
	ProteinSequenceSet protein_set(20140100, LogStatus::FULL_LOG);
	size_t suc_cnt = 0;
	if ((suc_cnt = protein_set.ParseUniprotXml("uniprot_sprot.xml")) > 0) //"/home/pswgoo/workspace/fdu_work/test_xmls/P31946.xml"
		cout << suc_cnt << " success" << endl;
	else
		cerr << "Failed" << endl;
	
	clog << "Complete" << endl;
	return 0;
}