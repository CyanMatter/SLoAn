#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <time.h>
#include <cstring>

#include <boost/filesystem.hpp>

#include <sys/stat.h>

#include "keynode.h"
#include "keytree.h"
#include "data.h"

using std::cout; using std::cin; 
using std::cerr; using std::endl;
using std::vector; using std::pair;
using std::string; using std::invalid_argument;
using std::make_shared; using std::shared_ptr;

bool fileExists(const path& name);
string queryInput();
string receiveInput(int max);
string parseInput(string input);
void solve(keytree* const& tree, data* const& map, const string input, const int min_solution_length = 2, const bool debug = false);
bool solve_rec(keytree* const& tree, shared_ptr<keynode> node_ptr, data* const& map, const string input, const int min_solution_length, const bool debug = false);
void maskSequence(const string seq, const int mask, string* const& subseq_in, string* const& subseq_out);
void eraseSolution(string key, unordered_map<string, vector<shared_ptr<keynode>>>* solutionMap);
bool contains(vector<shared_ptr<keynode>> map, string key);
bool checkSolutions(unordered_map<string, vector<shared_ptr<keynode>>>::const_iterator it, shared_ptr<keynode> node_ptr);
bool solve_intermediary_node_v1(const string& intermediary_key, shared_ptr<keynode> parent, const string& seq, const string& subseq_out, data* const& map, keytree* const& tree, const int min_solution_length, const bool debug);
bool solve_intermediary_node_v2(shared_ptr<keynode> intermediary_node_ptr, shared_ptr<keynode> parent, const string& seq, const string& subseq_out, data* const& map, keytree* const& tree, const int min_solution_length, const bool debug);
