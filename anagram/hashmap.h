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
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;
using namespace filesystem;

class hashmap
{
private:
	unordered_map<string, vector<string>> anagramMap;
	chrono::system_clock::time_point map_last_modified;
	path file_path;
	//path name;
	file_time_type file_last_modified;

public:
	unordered_map<string, vector<string>>& getAnagramMap();
	void setAnagramMap(unordered_map<string, vector<string>>);
	chrono::system_clock::time_point getMap_last_modified();
	void updateMap_last_modified();
	void setFile(const path&);
	path& getFile_path();
	void write();
	file_time_type getFile_last_modified();
	hashmap();

	static vector<string>* loadVocab(const path&);
	static void build(hashmap* const&);
};

