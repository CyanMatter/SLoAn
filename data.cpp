#include "data.h"

data::data()
{
	this->solutionMap = {};
}

unordered_map<string, vector<string>>* data::getAnagramMap()
{
	return &anagramMap;
}
void data::setAnagramMap(unordered_map<string, vector<string>> map)
{
	this->anagramMap = map;
}
unordered_map<string, vector<shared_ptr<keynode>>>* data::getSolutionMap()
{
	return &solutionMap;
}
time_t data::getMap_last_modified()
{
	return map_last_modified;
}
void data::setMap_last_modified(time_t t)
{
	this->map_last_modified = t;
}
void data::updateMap_last_modified()
{
	time(&map_last_modified);
	this->modified = true;
}
time_t data::getFile_last_modified()
{
	return file_last_modified;
}
void data::setFile_last_modified(time_t t)
{
	this->file_last_modified = t;
}
void data::setFile(const path& absolute_path)
{
	if (exists(absolute_path)) {
		this->file_path = absolute_path;
		setFile_last_modified(boost::filesystem::last_write_time(absolute_path));

		path wordlistPath = file_path;
		path mapName = wordlistPath.stem();
		mapName += ".dat";
		this->map_path = boost::filesystem::current_path() / "db" / "maps" / mapName;
	}
	else {
		string error_description = ("Could not open the file - '" + absolute_path.string()) + "'";
		throw new invalid_argument(error_description);
	}
}
path& data::getFile_path()
{
	return file_path;
}
path& data::getMap_path()
{
	return map_path;
}
size_t data::getLongestWord()
{
	return longestWord;
}
void data::setLongestWord(size_t x)
{
	this->longestWord = x;
}
bool data::isModified()
{
	return modified;
}
bool data::isSaved()
{
	return exists(map_path);
}

ofstream& operator<<(ofstream& ofs, data& map)
{
	ofs << "file_last_modified=" << endl << map.getFile_last_modified() << endl
		<< "map_last_modified=" << endl << map.getMap_last_modified() << endl
		<< "file_path=" << endl << map.getFile_path().string() << endl
		<< "longestWord=" << endl << map.getLongestWord() << endl
		<< "anagramMap={" << endl << map.unordered_map_as_string() << endl << '}';
	return ofs;
}

shared_ptr<keynode> data::addSolution(const string& solution_key, const string& node_key, const int min_solution_length)
{
	shared_ptr<keynode> node_ptr = make_shared<keynode>(keynode(node_key));
	this->addSolution(solution_key, node_ptr, min_solution_length);
	return node_ptr;
}

void data::addSolution(const string& solution_key, shared_ptr<keynode> node_ptr, const int min_solution_length)
{
	//!debug
	//!delete later
	string test_node = "lent";
	string test_solution = "parliament";
	sort(test_node.begin(), test_node.end());
	sort(test_solution.begin(), test_solution.end());
	if (test_node == node_ptr->key && test_solution == solution_key) {
		int a = 0;
	}
	
	if (solution_key.size() >= min_solution_length)
		this->solutionMap[solution_key].push_back(node_ptr);
}

void data::addEmptySolution(const string key, const int min_solution_length)
{
	if (key.size() >= min_solution_length)
		this->solutionMap[key] = vector<shared_ptr<keynode>>();
}

unordered_map<string, vector<shared_ptr<keynode>>>::const_iterator data::findSolution(const string& key)
{
	return this->solutionMap.find(key);
}

unordered_map<string, vector<string>>::const_iterator data::findAnagram(const string& key)
{
	return this->anagramMap.find(key);
}

bool data::eitherKeyIsInSolution(const string& key_x, const string& key_y, const string& seq)
{
	unordered_map<string, vector<shared_ptr<keynode>>>::const_iterator seq_it = this->solutionMap.find(seq);
	if (seq_it != this->solutionMap.end()) {
		for (shared_ptr<keynode> child : seq_it->second) {
			if (child->key == key_x || child->key == key_y) {
				return true;
			}
		}
	}
	return false;
}

void data::write(string& filename)
{
	try {
		path filepath = boost::filesystem::current_path() / "db" / "maps";
		if (!exists(filepath))
			create_directories(filepath);

		filepath /= filename.substr(0, filename.find_last_of('.')) + ".dat";
		ofstream file(filepath, std::ios::out);
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

time_t data::string_as_time_t(string& str)
{
	time_t t;
	std::stringstream ss;
	ss << str;
	ss >> t;
	return t;
}

string data::unordered_map_as_string()
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
		Size const n = std::char_traits<char>::length(&s[0]);
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
		v.push_back(line.substr(start, end - start));
		start = end + 1;
	}

	return v;
}

void readAnagramMap(data* const& map, ifstream& file)
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

void data::read(data* const& map, ifstream& file, bool debug)
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
				map->setFile((path)line);
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
		cout << "Sourced the data from the archived map at " << map->getMap_path() << endl;
}

/// <summary>
/// Reads the text file located at provided path and returns a vector of strings of which each entry is a line in the file
/// </summary>
/// <param name="path">The path to the text file to be loaded relative from the current working directory</param>
/// <returns>Vector of strings each line containing a line of the text file</returns>
vector<string>* data::loadVocab(const path& path)
{
	vector<string>* lines = new vector<string>();
	string word;

	ifstream input_file(path.string());
	if (!input_file.is_open()) {
		string error_description = ("Could not open the file - '" + path.string()) + "'";
		throw new invalid_argument(error_description);
	}

	while (getline(input_file, word)) {
		lines->push_back(word);
	}
	input_file.close();

	return lines;
}

void data::build(data* const& map, bool debug)
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
		anagramMap[key] = anagrams;						// store it in the data

		if (word.length() > longestWord)
			longestWord = word.length();				// update longest word size
	}

	map->setAnagramMap(anagramMap);
	map->setLongestWord(longestWord);

	if (debug)
		cout << "Sourced the data from the word list located at " << map->getFile_path() << endl;

	return;
}

bool data::storedIsValid(data* const& map, bool debug)
{
	path wordlistPath = map->file_path;
	path mapPath = map->map_path;

	if (!exists(mapPath))
		return false;

	time_t wordlist_last_modified = boost::filesystem::last_write_time(wordlistPath);
	time_t map_last_modified = boost::filesystem::last_write_time(mapPath);

	ifstream file(mapPath, std::ios::in);

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

time_t data::read_file_last_modified_for_map(data* const& map, ifstream& file)
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
