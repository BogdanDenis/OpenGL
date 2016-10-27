#include <string>
#include <fstream>

using std::string;
using std::getline;
using std::ifstream;

class FileReader {

public:
	FileReader ();

	~FileReader ();

	static string ReadShaderSource (string filePath);
};

