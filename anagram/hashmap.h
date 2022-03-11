#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <stddef.h>
#include "keynode.h"

typedef ptrdiff_t Size;

using namespace std;
using namespace filesystem;

class hashmap
{
private:
	unordered_map<string, vector<string>> anagramMap;
	unordered_map<string, keynode> solutionMap;
	time_t map_last_modified;
	time_t file_last_modified;
	path file_path;
	path map_path;
	size_t longestWord;
	bool modified;

public:
	unordered_map<string, vector<string>>& getAnagramMap();
	void setAnagramMap(unordered_map<string, vector<string>>);
	unordered_map<string, keynode>& getSolutionMap();
	time_t getMap_last_modified();
	void setMap_last_modified(time_t t);
	void updateMap_last_modified();
	time_t getFile_last_modified();
	void setFile_last_modified(time_t t);
	void setFile(const path&);
	path& getFile_path();
	path& getMap_path();
	void write(string& filename);
	friend ofstream& operator<<(ofstream& ofs, hashmap& map);
	string unordered_map_as_string();
	size_t getLongestWord();
	void setLongestWord(size_t x);
	bool isModified();
	bool isSaved();
	hashmap();
	
	static void read(hashmap* const& map, ifstream& file, bool debug = false);
	static vector<string>* loadVocab(const path& path);
	static void build(hashmap* const& map, bool debug = false);
	static bool storedIsValid(hashmap* const& map, bool debug = false);
	static time_t read_file_last_modified_for_map(hashmap* const& map, ifstream& file);
	static time_t string_as_time_t(string& str);
};

