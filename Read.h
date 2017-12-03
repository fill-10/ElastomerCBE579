#include <vector>
class Read {
public:
	Read() { }
	void setFileName(string name) { filename = name; }
	void openFile();
	vector<double> getLine();
	vector<double> getLine6();
	double getDouble();
	int getInt();
	void skipLine();
	void closeFile();
private:
	string filename;
	FILE* stream;
};
