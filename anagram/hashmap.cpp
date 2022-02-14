#include "hashmap.h"

hashmap::hashmap()
{
	updateMap_last_modified();
}

unordered_map<string, vector<string>>& hashmap::getAnagramMap()
{
	return anagramMap;
}
void hashmap::setAnagramMap(unordered_map<string, vector<string>> map)
{
	anagramMap = map;
	updateMap_last_modified();
}
time_t hashmap::getMap_last_modified()
{
	return map_last_modified;
}
void hashmap::updateMap_last_modified()
{
	time(&map_last_modified);
}
void hashmap::setFile(const path& absolute_path)
{
	file_path = absolute_path;
	file_last_modified = last_write_time(absolute_path);
	updateMap_last_modified();
}
path& hashmap::getFile_path()
{
	return file_path;
}
file_time_type hashmap::getFile_last_modified()
{
	return file_last_modified;
}
int hashmap::getLongestWord()
{
	return longestWord;
}
void hashmap::setLongestWord(int x)
{
	longestWord = x;
	updateMap_last_modified();
}

ofstream& operator<<(ofstream& ofs, hashmap map)
{
	//!WIP also include the other fields
	ofs << map.unordered_map_as_string();
	return ofs;
}

void hashmap::write(string& filename)
{
	try {
		path filepath = "db/bin/" + filename + ".dat";//!WIP can't find location
		ofstream file(filepath, ios::out);
		if (file.is_open()) {
			file << *this
				<< endl;
			file.close();
		}
		else
			cout << "Could not write to disk. Unable to open file " << filepath << endl;
	}
	catch (ofstream::failure e) {
		cout << e.what() << endl;
	}
}

string hashmap::unordered_map_as_string()
{
	//!WIP potential optimization https://stackoverflow.com/questions/48409391/faster-way-to-read-write-a-stdunordered-map-from-to-a-file/48412487#48412487
	string str = "";
	unordered_map<string, vector<string>>::iterator iter = anagramMap.begin();

	while (true) {
		str += iter->first.c_str();
		str += ':';
		
		vector<string> v = iter->second;
		int i = 0;
		while (true) {
			str += v.at(i);
			i++;
			if (i != v.size()) str += ',';
			else break;
		}

		iter++;
		if (iter != anagramMap.end()) str += '\n';
		else break;
	}

	str += '}';
	return str;
}

// WIP try to build boost serialize at home
// https://stackoverflow.com/questions/21141243/how-to-build-boost-serialization-library/21141851
// https://www.boost.org/doc/libs/1_78_0/tools/build/doc/html/index.html#bbv2.overview.invocation
// https://www.boost.org/doc/libs/1_78_0/more/getting_started/windows.html#prepare-to-use-a-boost-library-binary
/*
void hashmap::write()
{
	ofstream ofs("hashmap");
	boost::archive::binary_oarchive oa(ofs);
	oa << this->anagramMap;
}

void hashmap::load()
{
	this->anagramMap = {};
	{
		ifstream ifs("hashmap");
		boost::archive::binary_iarchive ia(ifs);
		ia >> this->anagramMap;
	}

	for (auto& p : this->anagramMap) {
		cout << p.first << " -> { ";
		copy(p.second.begin(), p.second.end(), ostream_iterator<string>(cout, " "));
		cout << "}\n";
	}
}
*/

// wrapper
ifstream& getline(ifstream& stream, string& s, Size const buf_size, char const delimiter = '\n')
{
	s.resize(buf_size);  assert(s.size() > 1);
	stream.getline(&s[0], buf_size, delimiter);
	if (!stream.fail())
	{
		Size const n = char_traits<char>::length(&s[0]);
		s.resize(n);      // Downsizing.
	}
	return stream;
}

vector<string> stringToVector(string& line, char const delimiter = ',')
{
	vector<string> v;
	size_t start = 0, end = 0;

	if (line == "ales,leas,sale,seal") {
		int a = 0;
	}

	while (end != string::npos) {
		end = line.find(delimiter, start);
		v.push_back(line.substr(start, end-start));
		start = end + 1;
	}

	return v;
}

void hashmap::read(hashmap* const& map, string filepath)
{
	try {
		ifstream file(filepath, ios::in);
		unordered_map<string, vector<string>> anagramMap = {};
		
		for (string line; getline(file, line, *(new Size(64)));) {
			size_t split = line.find(':');
			string key = line.substr(0, split);
			line.erase(0, split+1);
			vector<string> anagrams = stringToVector(line);
			anagramMap[key] = anagrams;
		}
		map->setAnagramMap(anagramMap);
	}
	catch (ifstream::failure e) {
		cout << e.what() << endl;
	}
	
	return;
}

/// <summary>
/// Reads the text file located at provided path and returns a vector of strings of which each entry is a line in the file
/// </summary>
/// <param name="path">The path to the text file to be loaded relative from the current working directory</param>
/// <returns>Vector of strings each line containing a line of the text file</returns>
vector<string>* hashmap::loadVocab(const path& path)
{
	vector<string>* lines = new vector<string>();
	string word;

	ifstream input_file(path.u8string());
	if (!input_file.is_open()) {
		string error_description = ( "Could not open the file - '" + path.string()) + "'";
		throw new invalid_argument(error_description);
	}

	while (getline(input_file, word)) {
		lines->push_back(word);
	}
	input_file.close();

	return lines;
}

void hashmap::build(hashmap* const& map) {
	vector<string>* vocab = loadVocab(map->getFile_path());
	unordered_map<string, vector<string>> anagramMap = {};
	int longestWord = 0;

	for (int i = 0; i < (*vocab).size(); i++) {
		string word = (*vocab)[i];						// for each word in vocabulary

		if (word.length() == 1 && tolower(word[0]) != 'a' && tolower(word[0]) != 'i')
			continue;									// skip all single letter words except 'a' and 'i'

		string key = word;								// copy the word to new variable
		sort(key.begin(), key.end());					// alphabetically sort key
		if (key[0] == '%') key = key.substr(1);			// eliminate '%' (indicating a multiple) from key

		vector<string> anagrams;						// initialize empty vector
		if (anagramMap.find(key) != anagramMap.end()) {	// if key already exists in map
			anagrams = anagramMap[key];					// refer 'anagrams' to the value associated with the key
		}
		anagrams.insert(anagrams.end(), word);			// add 'word' to the array of its anagrams
		anagramMap[key] = anagrams;						// store it in the hashmap

		if (word.length() > longestWord)
			longestWord = word.length();				// update longest word size
	}
	
	map->setAnagramMap(anagramMap);
	map->setLongestWord(longestWord);
	return;
}
;