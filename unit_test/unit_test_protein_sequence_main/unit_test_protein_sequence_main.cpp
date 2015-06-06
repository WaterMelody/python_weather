#include <iostream>
#include <string>
#include <vector>

#include "data_class/protein_sequence.h"

using namespace std;


int main() {
	ProteinSequence protein;
	if (protein.ParseXml("/home/pswgoo/workspace/fdu_work/test_xmls/P31946.xml") > 0)
		cout << protein.ToString() << endl;
	else
		cerr << "Failed" << endl;
	
	clog << "Complete" << endl;
	return 0;
}