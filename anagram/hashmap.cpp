#include "hashmap.h"

hashmap::hashmap()
{
	this->solutionMap = {};
}

template <typename TP>
time_t to_time_t(TP tp)
{
	using namespace chrono;
	auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
		+ system_clock::now());
	return system_clock::to_time_t(sctp);
}

unordered_map<string, vector<string>>& hashmap::getAnagramMap()
{
	return anagramMap;
}
void hashmap::setAnagramMap(unordered_map<string, vector<string>> map)
{
	this->anagramMap = map;
}
unordered_map<string, keynode>& hashmap::getSolutionMap()
{
	return solutionMap;
}
time_t hashmap::getMap_last_modified()
{
	return map_last_modified;
}
void hashmap::setMap_last_modified(time_t t)
{
	this->map_last_modified = t;
}
void hashmap::updateMap_last_modified()
{
	time(&map_last_modified);
	this->modified = true;
}
time_t hashmap::getFile_last_modified()
{
	return file_last_modified;
}
void hashmap::setFile_last_modified(time_t t)
{
	this->file_last_modified = t;
}
void hashmap::setFile(const path& absolute_path)
{
	if (exists(absolute_path)) {
		this->file_path = absolute_path;
		setFile_last_modified(to_time_t(last_write_time(absolute_path)));

		path wordlistPath = file_path;
		path mapName = wordlistPath.stem();
		mapName += ".dat";
		this->map_path = current_path() / "db" / "maps" / mapName;
	}
}
path& hashmap::getFile_path()
{
	return file_path;
}
path& hashmap::getMap_path()
{
	return map_path;
}
size_t hashmap::getLongestWord()
{
	return longestWord;
}
void hashmap::setLongestWord(size_t x)
{
	this->longestWord = x;
}
bool hashmap::isModified()
{
	return modified;
}
bool hashmap::isSaved()
{
	return exists(map_path);
}

ofstream& operator<<(ofstream& ofs, hashmap& map)
{
	ofs << "file_last_modified=" << endl << map.getFile_last_modified() << endl
		<< "map_last_modified=" << endl << map.getMap_last_modified() << endl
		<< "file_path=" << endl << map.getFile_path().string() << endl
		<< "longestWord=" << endl << map.getLongestWord() << endl
		<< "anagramMap={" << endl << map.unordered_map_as_string() << endl << '}';
	return ofs;
}

void hashmap::write(string& filename)
{
	try {
		path filepath = current_path() / "db" / "maps";
		if (!exists(filepath))
			create_directories(filepath);

		filepath /= filename.substr(0, filename.find_last_of('.')) + ".dat";
		ofstream file(filepath, ios::out);
		if (file.is_open()) {
			file << *this;
			file.close();
		}
		else
			cout << "Could not write to disk. Unable to open file " << filepath << endl;
	}
	catch (ofstream::failure e) {
		cout << e.what() << endl;
	}
}

time_t hashmap::string_as_time_t(string& str)
{
	time_t t;
	stringstream ss;
	ss << str;
	ss >> t;
	return t;
}

string hashmap::unordered_map_as_string()
{
	//TODO potential optimization https://stackoverflow.com/questions/48409391/faster-way-to-read-write-a-stdunordered-map-from-to-a-file/48412487#48412487
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

	while (end != string::npos) {
		end = line.find(delimiter, start);
		v.push_back(line.substr(start, end-start));
		start = end + 1;
	}

	return v;
}

void readAnagramMap(hashmap* const& map, ifstream& file)
{
	unordered_map<string, vector<string>> anagramMap = {};

	for (string line; line != "}"; getline(file, line)) {
		size_t split = line.find(':');
		string key = line.substr(0, split);
		line.erase(0, split + 1);
		vector<string> anagrams = stringToVector(line);
		anagramMap[key] = anagrams;
	}

	map->setAnagramMap(anagramMap);
}

void hashmap::read(hashmap* const& map, ifstream& file, bool debug)
{
	try {
		for (string line; getline(file, line);) {
			if (line == "file_last_modified=") {
				getline(file, line);
				map->setFile_last_modified(string_as_time_t(line));
			}
			else if (line == "map_last_modified=") {
				getline(file, line);
				map->setMap_last_modified(string_as_time_t(line));
			}
			else if (line == "file_path=") {
				getline(file, line);
				map->setFile((path) line);
			}
			else if (line == "longestWord=") {
				getline(file, line);
				map->setLongestWord(stoi(line));
			}
			else if (line == "anagramMap={") {
				readAnagramMap(map, file);
			}
		}
	}
	catch (ifstream::failure e) {
		cout << e.what() << endl;
	}

	if (debug)
		cout << "Sourced the hashmap from the archived map at " << map->getMap_path() << endl;
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

void hashmap::build(hashmap* const& map, bool debug)
{
	if (storedIsValid(map, debug))						// if a stored version of this wordlist exists, and it is up-to-date,
		return;											// then build the map from there instead

	vector<string>* vocab = loadVocab(map->getFile_path());
	unordered_map<string, vector<string>> anagramMap = {};
	size_t longestWord = 0;

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

	if (debug)
		cout << "Sourced the hashmap from the word list located at " << map->getFile_path() << endl;

	return;
}

bool hashmap::storedIsValid(hashmap* const& map, bool debug)
{
	path wordlistPath = map->file_path;
	path mapPath = map->map_path;

	if (!exists(mapPath))
		return false;
	
	time_t wordlist_last_modified = to_time_t(last_write_time(wordlistPath));
	time_t map_last_modified = to_time_t(last_write_time(mapPath));

	ifstream file(mapPath, ios::in);

	if (wordlist_last_modified < map_last_modified) {
		read(map, file, debug);
		return true;
	}

	time_t file_last_modified_for_map = read_file_last_modified_for_map(map, file);
	if (wordlist_last_modified < file_last_modified_for_map) {
		read(map, file, debug);
		return true;
	}
	else 
		return false;
}

time_t hashmap::read_file_last_modified_for_map(hashmap* const& map, ifstream& file)
{
	string line;
	getline(file, line);
	if (line == "file_last_modified=") {						// This should be the first line in the file
		getline(file, line);
		time_t file_last_modified_for_map = string_as_time_t(line);
		map->setFile_last_modified(file_last_modified_for_map);
		return file_last_modified_for_map;
	}
	else
		throw new invalid_argument("Unable to read the first line of input file");
}
;