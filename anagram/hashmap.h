#pragma once
/*
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>
*/
#include <unordered_map>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <assert.h>
#include <stddef.h>

typedef ptrdiff_t Size;

using namespace std;
using namespace filesystem;

class hashmap
{
private:
	unordered_map<string, vector<string>> anagramMap;
	time_t map_last_modified;
	time_t file_last_modified;
	path file_path;
	int longestWord;

public:
	unordered_map<string, vector<string>>& getAnagramMap();
	void setAnagramMap(unordered_map<string, vector<string>>);
	time_t getMap_last_modified();
	void setMap_last_modified(time_t t);
	void updateMap_last_modified();
	time_t getFile_last_modified();
	void setFile_last_modified(time_t t);
	void setFile(const path&);
	path& getFile_path();
	void write(string& filename);
	friend ofstream& operator<<(ofstream& ofs, hashmap& map);
	string unordered_map_as_string();
	int getLongestWord();
	void setLongestWord(int x);
	hashmap();
	
	static void read(hashmap* const& map, ifstream& file);
	static vector<string>* loadVocab(const path& path);
	static void build(hashmap* const& map);
	static bool storedIsValid(hashmap* const& map);
	static time_t read_file_last_modified_for_map(hashmap* const& map, ifstream& file);
	static time_t string_as_time_t(string& str);
};

