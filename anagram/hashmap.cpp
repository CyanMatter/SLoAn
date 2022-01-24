#include "hashmap.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>
#include <filesystem>
#include <fstream>

using namespace std;
using namespace filesystem;

class hashmap
{
public:

	hashmap(unordered_map<string, vector<string>> anagramMap, const path file_path)
	{
		const unordered_map<string, vector<string>> anagramMap = anagramMap;
		const chrono::system_clock::time_point last_modified = chrono::system_clock::now();
		const path file_path = file_path;
		//const path name = file_path.filename();
		const file_time_type file_last_modified = last_write_time(file_path);
	}

	/// <summary>
	/// Reads the text file located at provided path and returns a vector of strings of which each entry is a line in the file
	/// </summary>
	/// <param name="path">The path to the text file to be loaded relative from the current working directory</param>
	/// <returns>Vector of strings each line containing a line of the text file</returns>
	vector<string>* loadVocab(const path& path)
	{
		vector<string>* lines = new vector<string>();
		string word;

		ifstream input_file(path.u8string());
		if (!input_file.is_open()) {
			cerr << "Could not open the file - '"//replace with try/catch
				<< path << "'" << endl;
			return lines;
		}

		while (getline(input_file, word)) {
			(*lines).push_back(word);
		}
		input_file.close();

		return lines;
	}

	hashmap build(const path& file_path) {
		vector<string>* vocab = loadVocab(file_path);

		std::unordered_map<string, vector<string>> anagramMap = {};
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


		return new hashmap(anagramMap, file_path);
	}
};