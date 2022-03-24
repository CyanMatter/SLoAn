#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <stddef.h>
#include <algorithm>

#include <boost/filesystem.hpp>
#include <boost/chrono.hpp>

#include "keynode.h"

typedef ptrdiff_t Size;

using boost::filesystem::path;
using boost::filesystem::ofstream;
using boost::filesystem::ifstream;
using boost::chrono::time_point_cast;
using boost::chrono::system_clock;
using std::endl; using std::make_shared;
using std::cout; using std::invalid_argument;

class data
{
private:
	unordered_map<string, vector<string>> anagramMap;
	unordered_map<string, vector<shared_ptr<keynode>>> solutionMap;
	time_t map_last_modified;
	time_t file_last_modified;
	path file_path;
	path map_path;
	size_t longestWord;
	bool modified;

public:
	unordered_map<string, vector<string>>* getAnagramMap();
	void setAnagramMap(unordered_map<string, vector<string>>);
	unordered_map<string, vector<shared_ptr<keynode>>>* getSolutionMap();
	time_t getMap_last_modified();
	void setMap_last_modified(time_t t);
	void updateMap_last_modified();
	time_t getFile_last_modified();
	void setFile_last_modified(time_t t);
	void setFile(const path&);
	path& getFile_path();
	path& getMap_path();
	void write(string& filename);
	friend ofstream& operator<<(ofstream& ofs, data& map);
	string unordered_map_as_string();
	size_t getLongestWord();
	void setLongestWord(size_t x);
	bool isModified();
	bool isSaved();
	shared_ptr<keynode> addSolution(const string& solution_key, const string& node_key, const int min_solution_length);
	void addSolution(const string& solution_key, shared_ptr<keynode> node_ptr, const int min_solution_length);
	void addEmptySolution(const string key, const int min_solution_length);
	unordered_map<string, vector<shared_ptr<keynode>>>::const_iterator findSolution(const string& key);
	unordered_map<string, vector<string>>::const_iterator findAnagram(const string& key);
	bool eitherKeyIsInSolution(const string& key_x, const string& key_y, const string& seq);

	data();

	static void read(data* const& map, ifstream& file, bool debug = false);
	static vector<string>* loadVocab(const path& path);
	static void build(data* const& map, bool debug = false);
	static bool storedIsValid(data* const& map, bool debug = false);
	static time_t read_file_last_modified_for_map(data* const& map, ifstream& file);
	static time_t string_as_time_t(string& str);
};