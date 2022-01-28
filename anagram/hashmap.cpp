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
chrono::system_clock::time_point hashmap::getMap_last_modified()
{
	return map_last_modified;
}
void hashmap::updateMap_last_modified()
{
	map_last_modified = chrono::system_clock::now();
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
		(*lines).push_back(word);
	}
	input_file.close();

	return lines;
}

void hashmap::build(hashmap* const& map) {
	vector<string>* vocab = loadVocab((*map).getFile_path());
	unordered_map<string, vector<string>> anagramMap = {};

	for (int i = 0; i < (*vocab).size(); i++) {
		string word = (*vocab)[i];						// for each word in vocabulary

		if (word.length() == 1 && tolower(word[0]) != 'a' && tolower(word[0]) != 'i')
			continue;									// skip all single letter words except 'a' and 'i'

		string key = word;								// copy the word to new variable
		sort(key.begin(), key.end());					// alphabetically sort key

		vector<string> anagrams;						// initialize empty vector
		if (anagramMap.find(key) != anagramMap.end()) {	// if key already exists in map
			anagrams = anagramMap[key];					// refer 'anagrams' to the value associated with the key
		}
		anagrams.insert(anagrams.end(), word);			// add 'word' to the array of its anagrams
		anagramMap[key] = anagrams;						// store it in the hashmap
	}
	
	(*map).setAnagramMap(anagramMap);
	return;
}
;