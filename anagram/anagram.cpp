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
unordered_map<vector<unsigned long long>, vector<string>*>* buildMap(vector<string>* vocab);
vector<unsigned long long> toBitboards(const string& word);
int collectCount(unsigned int letter, vector<unsigned long long>, const int nBoards);

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

	unordered_map<vector<unsigned long long>, vector<string>*>* anagramMap;
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

unordered_map<vector<unsigned long long>, vector<string>*>* buildMap(vector<string>* vocab)
{
	unordered_map<vector<unsigned long long>, vector<string>*> anagramMap;

	for (int i = 0; i < (*vocab).size(); i++) {												// iterate over each word in vocab list
		const string word = (*vocab)[i];

		if (word.size() == 1 && (tolower(word[0]) != 'a' || tolower(word[0]) != 'i'))		// filter out all single letter "words," except a and i
			continue;

		const vector<unsigned long long> key = toBitboards(word);							// the bitboard array of this word functions as a key in the hashmap
		if (anagramMap.count(key) > 0) {													// if the key yet exists in the hashmap
			vector<string>* anagramList = anagramMap[key];								// retrieve the list of anagrams that belong to this key
			(*anagramList).push_back(word);													// add this word to that list of anagrams
		}
		else {																				// else, the key does not yet exist in the hashmap
			vector<string>* newAnagramList = new vector<string>();							// create a new list of anagrams
			(*newAnagramList).push_back(word);												// add this word as the first anagram to the new list
			anagramMap.insert({ key, newAnagramList });									// add it as a new key-value pair to the hashmap
		}
		// WIP
	}

	return &anagramMap;
}


/// <summary>
/// Accepts a string and converts it to a vector of bitboards that represents it.
/// The bitboards are ordered such that, for any bitboard, two bits are reserved for each letter in alphabetical order.
/// Together the bitboards form a binary number at their i-th index for the i-th symbol in the alphabet
/// Sequencing those two bits from each bitboard results in a number that equals the amount that letter occurs in the word.
/// For example, if the letter 'a' occurs 11 times in a word, the trailing two bits of each bitboard from first to last will read: 11 10.
/// </summary>
/// <param name="word">The string to convert</param>
/// <returns>Vector of bitboards that represent the string</returns>
vector<unsigned long long> toBitboards(const string& word)
{
	unsigned int letter, temp;
	unsigned long long count, index;
	const unsigned int mask = 3;
	const unsigned int nBoards = 2;
	const unsigned int maxCount = (1 << (nBoards << 1)) - 1;
	vector<unsigned long long>* bitboards = new vector<unsigned long long>(nBoards, 0);	// Each bitboard needs 26 bits, one for each alphabetic symbol
	unsigned long long letterCounts[26] = { 0ull };	//!todo only include occurring letters in map-like structure

	for (int i = 0; i < word.size(); i++) {							// iterate over each letter in word; we are going to collect letter occurences in letterCounts
		if (isalpha(word[i])) {										// exclude any non-alphabetic symbols
			letter = ((unsigned long long) tolower(word[i])) - 97;	// convert letter to appropriate value (97 is binary value of char a)
			letterCounts[letter]++;									// increment the count of this letter
		}
	}

	for (int letter = 0; letter < 26; letter++) {					// iterate over each letter in the alphabet
		count = letterCounts[letter];								// get amount of occurences of this letter in the word

		if (count > maxCount) {										// exception if there aren't enough bitboards in the array
			EXIT_FAILURE; // integer overflow
			//todo: substitute with dynamic bitboards array size
		}

		if (count > 0) {											// exclude any letters that don't occur in the word
			index = 1ull << (letter << 1);							// 'index' is the location in the bitboard where the space reserved for this letter starts
			for (int b = 0; b < nBoards; b++) {						// iterate over each bitboard in the array 'bitboards'
				temp = count >> (b << 1);							// 'temp' holds a temporary value used in the next step
				(*bitboards)[b] += (temp & mask) * index;			// store the value that identifies this letter for this word in this bitboard
			}
		}
	}

	return (*bitboards);											// return the array of bitboards that represents the provided word
}

/// <summary>
/// Collects the count of a letter stored in an array of bitboards. Mainly used for debugging
/// </summary>
/// <param name="letter">The letter of which the count should be retrieved</param>
/// <param name="bitboards">The array of bitboards in which the counter of the letter is stored</param>
/// <param name="nBoards">The size of the bitboards array, i.e. the number of bitboards in the array</param>
/// <returns>An integer with the number of occurences of the letter in any word the array of bitboards represents</returns>
int collectCount(const unsigned int letter, vector<unsigned long long> bitboards, const int nBoards)
{
	int count = 0;
	const int letterIndex = (letter - 97) << 1;
	for (unsigned long long i = 0; i < nBoards; i++)
	{
		int shiftedBitboard = (unsigned long long) bitboards[i] >> letterIndex;		// shift this bitboard so the bits for this letter are at index 0 and 1
		int maskedBitboard = shiftedBitboard & 3ull;								// mask this bitboard so that all irrelevant bits are set to 0
		count += maskedBitboard << (i << 1ull);										// calculate how much this bitboard counts towards the total count
	}
	return count;
}