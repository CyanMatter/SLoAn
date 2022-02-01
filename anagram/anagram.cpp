#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <string>
#include <time.h>

#include "hashmap.h"

using namespace std;

namespace fs = filesystem;

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::vector;

bool fileExists(const string& name);
string queryInput();
string receiveInput(int input);
string parseInput(string input);
void findAndLogAnagrams(hashmap anagramMap, string input, bool debug);

int main(int argc, char* argv[])
{
	const bool debug = true;
	string path_to_wordlist = "db\\2of12inf.txt";
	hashmap* const& map = new hashmap();

	bool lookingForFile = true;
	while (lookingForFile) {
		string absolute_path_to_wordlist = fs::current_path().string() + "\\" + path_to_wordlist;
		if (!fileExists(path_to_wordlist))
		{
			cout << "Cannot open " + absolute_path_to_wordlist;
			EXIT_FAILURE;
		}

		(*map).setFile(absolute_path_to_wordlist);
		cout << "Loading " << absolute_path_to_wordlist << "...";
		clock_t t = clock();
		try {
			hashmap::build(map);
		}
		catch (invalid_argument& e) {
			cout << e.what();
			while (bool query = true) {
				char input[1];
				cout << endl << "Retry? (y/n): ";
				cin >> input;
				if (tolower(input[0]) == 'y') {
					query = false;
				}
				else if (tolower(input[0]) == 'n') {
					cout << endl << "Program closed";
					EXIT_SUCCESS;
				}
			}
			continue;
		}
		lookingForFile = false;

		t = clock() - t;

		if (debug)
			cout << "Constructed hashmap in approximately " << (float)t / CLOCKS_PER_SEC << "seconds\n";

		map->write();
	}

	string input = queryInput();
	findAndLogAnagrams((*map), input, debug);

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
			if (invalid_chars.find(c) != string::npos) {	// if c is not yet in invalid_chars
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

string maskString(string& str, int mask[])
{
	int size = sizeof(mask) / sizeof(mask[0]);
	string sub_str;
	for (int i = 0; i < size; i++) {
		if (mask[i] == 1)
			sub_str += str.at(i);
	}
	return sub_str;
}

// deprecates finAndLogAnagrams
void solveAnagrams(hashmap& hashmap, string input, bool debug = false)
{
	int n = (1 << input.length());
	for (int mask = 1; mask < n; mask++) {
		// should be int array
	}
}

void findAndLogAnagrams(hashmap hashmap, string input, bool debug = false)
{
	clock_t t = clock();
	auto anagramMap = hashmap.getAnagramMap();
	string key = input;
	sort(key.begin(), key.end());
	auto iterator = anagramMap.find(key);
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