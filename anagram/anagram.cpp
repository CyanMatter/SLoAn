#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <string>
#include <time.h>

#include "hashmap.h"
#include "keytree.h"
#include "keynode.h"

using namespace std;

namespace fs = filesystem;

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::vector;

bool fileExists(const string& name);
string queryInput();
string receiveInput(int max);
string parseInput(string input);
bool solveAnagrams(keytree* const& tree, keynode& node, unordered_map<string, vector<string>>* const& anagramMap, string input, bool debug = false);
//void findAndLogAnagrams(hashmap hashmap, string input, bool debug);

int main(int argc, char* argv[])
{
	const bool debug = true;
	string path_to_wordlist = "db\\2of12inf.txt";
	hashmap* const& map = new hashmap();

	bool lookingForFile = true;
	while (lookingForFile) {
		string absolute_path_to_wordlist = fs::current_path().string() + "\\" + path_to_wordlist;
		if (!fileExists(absolute_path_to_wordlist))
		{
			cout << "Cannot open " + absolute_path_to_wordlist << endl;
			EXIT_FAILURE;
		}

		map->setFile(absolute_path_to_wordlist);
		cout << "Loading " << absolute_path_to_wordlist << "..." << endl;
		clock_t t = clock();
		try {
			hashmap::build(map, debug);
		}
		catch (invalid_argument& e) {
			cout << e.what() << endl;
			while (bool query = true) {
				char input[1];
				cout << "Retry? (y/n): ";
				cin >> input;
				cout << endl;
				if (tolower(input[0]) == 'y') {
					query = false;
				}
				else if (tolower(input[0]) == 'n') {
					cout << "Program closed" << endl;
					EXIT_SUCCESS;
				}
			}
			continue;
		}
		lookingForFile = false;

		t = clock() - t;

		if (debug)
			cout << "Constructed hashmap in approximately " << (float)t / CLOCKS_PER_SEC << "seconds" << endl;

		string filename = map->getFile_path().filename().string();
		if (map->isModified() || !map->isSaved()) {
			map->write(filename);
		}
	}

	string input = queryInput();
	//findAndLogAnagrams((*map), input, debug);

	keytree* const& tree = new keytree();
	string sorted_input = parseInput(input);										// allow only permitted characters in input string
	sort(sorted_input.begin(), sorted_input.end());									// sort input string in alphabetical order
	solveAnagrams(tree, tree->root, &(map->getAnagramMap()), sorted_input, debug);
	vector<vector<string>> solution_arr = tree->traverse();

	for (int i = 0; i < solution_arr.size(); i++) {
		cout << "Solution " << i << ":" << endl;
		vector<string> solution = solution_arr[i];

		for (string key : solution) {
			if (key == "")
				break;

			auto iterator = map->getAnagramMap().find(key);
			vector<string> anagrams = iterator->second;
			string line = "\t[";
			for (string anagram : anagrams) {
				line += anagram + ", ";
			}
			line = line.substr(0, line.size() - 2) + "]";
			cout << line << endl;
		}
	}

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

bool solveAnagrams(keytree* const& tree, keynode& node, unordered_map<string, vector<string>>* const& anagramMap, string input, bool debug)
{
	bool is_solution = false;
	int n = 1 << input.length();										// n is the maximum iterations	
	for (int mask = 1; mask < n; mask++) {								// mask will be every possible configuration of 0's and 1's in an size n binary sequence (except the sequence = 0)
		
		int d = mask;													// d is the dividend
		int r = 0;														// r is the remainder
		unsigned long long i = 0ull;									// i is the index of the current character of input
		string subseq_in = "";											// subseq_in is the subsequence of input that is in the current mask
		string subseq_out = "";											// subseq_out is the subsequence of input that is out the current mask

		while (d > 0) {													// in this loop we read out every binary digit in the binary sequence of mask
			r = d % 2;													// read out the digit and store in r
			d >>= 1;													// shift to the next (which is the same as: divide by 2)
			((r == 1) ? subseq_in : subseq_out) += input.at(i);			// store the char in either subseq_in or subseq_out according to the mask
			i++;														// increment i and iterate
		}
		auto iterator = anagramMap->find(subseq_in);					// get all anagrams of subseq_in
		if (iterator != anagramMap->end()) {							// if there is any anagram at all, then we'll go a recursion deeper
			keynode child = keynode(subseq_in, node.depth + 1);			// create a child node containing the key for the known anagram
			if (input.size() > i)
				subseq_out += input.substr(i);							// if any, append remaining characters of input to subseq_out
			if (subseq_out.size() == 0) {								// if all letters in the input have been used in the sequence
				tree->addChild(child, node);							// add the new child node to this node
				return true;											// indicate to caller that a solution has been found
			}
			else if (solveAnagrams(tree, child, anagramMap, subseq_out, debug)) {	// if all parts in the sequence form a solution
				is_solution = true;
				tree->addChild(child, node);							// then add all those parts to this node
			}															// continue looking for other solutions
		}
	}
	if (is_solution) {
		int a = 0;
	}
	return is_solution;
}

/*
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
		bool inputInList = false;
		vector<string> anagrams = iterator->second;
		vector<string>::iterator input_index = find(anagrams.begin(), anagrams.end(), input);		// try to find input text in anagrams
		if (input_index != anagrams.end()) {														// in case input text in anagrams
			anagrams.erase(input_index);															// filter input text from anagrams
			inputInList = true;
		}

		const unsigned int size = anagrams.size();
		cout << size << " anagram" << ((size != 1) ? "s" : "") << " found";
		if (debug) {
			printf(" in approximately %f seconds", t / CLOCKS_PER_SEC);
			string found_str = "Input text is";
			found_str += inputInList ? " " : " not ";
			found_str += "found in the current list";
			printf((char*) &found_str);
		}
		cout << "\n";
		for (string word : anagrams) {
			if (word != input) {
				cout << word << "\n";
			}
		}
	}
}
*/