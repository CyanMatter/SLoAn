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
	path file_path;
	//path name;
	file_time_type file_last_modified;
	int longestWord;

public:
	unordered_map<string, vector<string>>& getAnagramMap();
	void setAnagramMap(unordered_map<string, vector<string>>);
	time_t getMap_last_modified();
	void updateMap_last_modified();
	void setFile(const path&);
	path& getFile_path();
	void write(string& filename);
	friend ofstream& operator<<(ofstream& ofs, hashmap map);
	string unordered_map_as_string();
	file_time_type getFile_last_modified();
	int getLongestWord();
	void setLongestWord(int x);
	hashmap();
	
	static void read(hashmap* const& map, string filepath);
	static vector<string>* loadVocab(const path&);
	static void build(hashmap* const&);
};

