#include "FileReader.h"

FileReader::FileReader () {

}


FileReader::~FileReader () {

}

string FileReader::ReadShaderSource (string filePath) {
	ifstream in (filePath);
	string source = "";
	while (!in.eof ()) {
		string line;
		getline (in, line);
		source += line;
		source.push_back ('\n');
	}
	return source;
}
