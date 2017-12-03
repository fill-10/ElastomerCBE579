/*
 *  VTF.h
 *  A class to write VTF files; readable with VMD
 *
 */

class VTFFile {
public:
	VTFFile() { box = 0; }
	~VTFFile();
public:
	void setFileName(string fn) { filename = fn; }
	void setBox(Box* _box) {
		box = _box;
	}
	void setNumberOfPolymers(int nop) {
		number_of_polymers = nop;
		assert(box);
		for(int i=0;i<number_of_polymers;i++) {
			polymers.push_back(box->getPolymer(i));
		}
	}
public:
	void writeHeader();
	void writeBeadPositions();
private:
	vector<Polymer*> polymers;
	int number_of_polymers;
	Box* box;
	FILE* stream;
	string filename;
};
