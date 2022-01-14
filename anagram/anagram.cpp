#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

namespace fs = filesystem;

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::vector;

bool fileExists(const string& name);
vector<string>* loadVocab(const string& path);
std::unordered_map<string, vector<string>> buildMap(vector<string>* vocab);

int main()
{
	string path_to_wordlist = "db\\3esl.txt";
	if (!fileExists(path_to_wordlist))
	{
		string absolute_path = fs::current_path().string() + "\\" + path_to_wordlist;
		cout << "Cannot open " + absolute_path;
		EXIT_FAILURE;
	}
	vector<string>* vocab = loadVocab(path_to_wordlist);

	std::unordered_map<string, vector<string>> anagramMap;
	anagramMap = buildMap(vocab);

	return 0;
}

/// <summary>
/// Returns true if a file exists at the provided file path.
/// The path is relative from the current working directory.
/// </summary>
/// <param name="name">The path to the file to be checked relative from the current working directory</param>
/// <returns>True, if a file exists at the location and false otherwise</returns>
bool fileExists(const string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

/// <summary>
/// Reads the text file located at provided path and returns a vector of strings of which each entry is a line in the file
/// </summary>
/// <param name="path">The path to the text file to be loaded relative from the current working directory</param>
/// <returns>Vector of strings each line containing a line of the text file</returns>
vector<string>* loadVocab(const string& path)
{
	vector<string>* lines = new vector<string>();
	string word;

	ifstream input_file(path);
	if (!input_file.is_open()) {
		cerr << "Could not open the file - '"
			<< path << "'" << endl;
		return lines;
	}

	while (getline(input_file, word)) {
		(*lines).push_back(word);
	}
	input_file.close();

	return lines;
}

std::unordered_map<string, vector<string>> buildMap(vector<string>* vocab)
{
	std::unordered_map<string, vector<string>> anagramMap = {};
	
	for (int i = 0; i < (*vocab).size(); i++) {
		string word = (*vocab)[i];						// for each word in vocabulary
		string key = word;								// copy the word to new variable
		sort(key.begin(), key.end());					// alphabetically sort key

		vector<string> anagrams;						// initialize empty vector
		if (anagramMap.find(key) != anagramMap.end()) {	// if key already exists in map
			anagrams = anagramMap[key];					// refer 'anagrams' to the value associated with the key
		}
		anagrams.insert(anagrams.end(), word);			// add 'word' to the array of its anagrams
		anagramMap[key] = anagrams;						// store it in the hashmap
	}

	return anagramMap;
}
