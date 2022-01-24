#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <string>
#include <time.h>
using namespace std;

namespace fs = filesystem;

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::vector;

bool fileExists(const string& name);
vector<string>* loadVocab(const string& path);
unordered_map<string, vector<string>> buildMap(vector<string>* vocab);
string queryInput();
string receiveInput(int input);
string parseInput(string input);
void findAndLogAnagrams(unordered_map<string, vector<string>> anagramMap, string input, bool debug);

int main(int argc, char* argv[])
{
	const bool debug = true;
	string path_to_wordlist = "db\\2of12inf.txt";

	string absolute_path_to_wordlist = fs::current_path().string() + "\\" + path_to_wordlist;
	if (!fileExists(path_to_wordlist))
	{
		cout << "Cannot open " + absolute_path_to_wordlist;
		EXIT_FAILURE;
	}
	cout << "Loading " << absolute_path_to_wordlist << "...";
	clock_t t = clock();
	vector<string>* vocab = loadVocab(path_to_wordlist);
	unordered_map<string, vector<string>> anagramMap = buildMap(vocab);
	t = clock() - t;

	if (debug)
		cout << "Constructed hashmap in approximately " << (float)t / CLOCKS_PER_SEC << "seconds\n";

	string input = queryInput();
	findAndLogAnagrams(anagramMap, input, debug);

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

	return anagramMap;
}

string queryInput()
{
	int max = 64;
	bool validInput = false;
	string input;

	while (!validInput) {
		try {
			cout << "Enter text: ";
			input = receiveInput(max);
			cout << "\n";
			input = parseInput(input);
			validInput = true;
		}
		catch (const std::invalid_argument& e)
		{
			cout << &e << "\n";
		}
	}

	return input;
}

string receiveInput(int max)
{
	if (max > 64)
		EXIT_FAILURE;									// input array cannot be initialized with a larger size than 64 elements
	
	char input[66];
	cin.get(input, 66);
	if (strlen(input) > max) {
		string error_msg = "Exceeded input character limit. No more than ";
		error_msg += max;
		error_msg += " characters are allowed";

		throw new invalid_argument(error_msg);
	}

	return string(input);
}

string parseInput(string input)
{
	string parsedInput, invalid_chars;
	bool invalid_argument = false;

	for (char const& c : input) {
		if (isalpha(c)) {						// alphabetic symbols are allowed and included
			parsedInput += c;
		}
		else if (isspace(c) || ispunct(c)) {	// whitespace and punctuation is allowed but excluded
			continue;
		}
		else {									// any other characters are not allowed
			invalid_argument = true;
			if (invalid_chars.find(c) != string::npos) {		// if c is not yet in invalid_chars
				invalid_chars += c;							// then add c
			}
		}
	}

	if (invalid_argument) {
		string error_msg;
		if (invalid_chars.length() == 1) {
			error_msg = "Symbol '" + invalid_chars + "' is not allowed as part of the input";
		}
		else {
			error_msg = "Several symbols are not allowed as part of the input: ";

			for (string::size_type i = 0; i < invalid_chars.length(); i++) {
				char c = invalid_chars[i];
				error_msg += "'" + c;
				error_msg += "'";
				if (invalid_chars.length() > i + 1) {
					error_msg += ", ";
				}
			}
		}
		throw std::invalid_argument(error_msg);
	}

	return parsedInput;
}

void findAndLogAnagrams(unordered_map<string, vector<string>> anagramMap, string input, bool debug = false) {
	clock_t t = clock();
	string key = input;
	sort(key.begin(), key.end());
	unordered_map<string, vector<string>>::const_iterator iterator = anagramMap.find(key);
	t = clock() - t;
	
	if (iterator == anagramMap.end()) {
		cout << "0 anagrams found";
		if (debug)
			printf(" in approximately %f seconds", t / CLOCKS_PER_SEC);
		cout << "\n";
	}
	else {
		vector<string> anagrams = iterator->second;
		vector<string>::iterator input_index = find(anagrams.begin(), anagrams.end(), input);		// try to find input text in anagrams
		if (input_index != anagrams.end()) {														// in case input text in anagrams
			anagrams.erase(input_index);															// filter input text from anagrams
		}

		const unsigned int size = anagrams.size();
		cout << size << " anagram" << ((size != 1) ? "s" : "") << " found";
		if (debug)
			printf(" in approximately %f seconds", t / CLOCKS_PER_SEC);
		cout << "\n";
		for (string word : anagrams) {
			if (word != input) {
				cout << word << "\n";
			}
		}
	}
}