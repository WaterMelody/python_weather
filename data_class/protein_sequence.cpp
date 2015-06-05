#include "protein_sequence.h" 

AminoType GetAminoType(const std::string &x) {
	for	(int name = AminoType::NON; name <= AminoType::VAL; ++name)
		if (kTypeString[name] == x || (x.length() == 1 && x[0] == kTypeChar[name]))
			return static_cast<AminoType>(name);
	return AminoType::NON;
}
