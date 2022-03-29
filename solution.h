#pragma once

#include <string>
#include <memory>
#include <vector>
#include <cassert>
#include <algorithm>

using std::vector; using std::string;
using std::shared_ptr; using std::make_shared;

typedef class solution;
typedef vector<shared_ptr<solution>> anagram;

class solution
{
public:	
	// Constructors
	solution();
	solution(const string& key);
	solution(const string& key, const vector<anagram> set, const int max_height, const int leaves, const bool is_leaf, const bool is_anagram);

	// Getter methods
	string getKey();
	vector<anagram> getSet();
	shared_ptr<solution> getPtr();

	// Public methods
	void add(const anagram new_anagram);
	vector<vector<string>> aggregate();

private:
	// Private attributes
	const shared_ptr<solution> ptr = make_shared<solution>(*this);
	bool is_empty;
	bool is_anagram;
	bool is_leaf;
	string key;
	size_t key_size;
	vector<anagram> set;
	int max_height;
	int leaves;

	// Private methods
	const bool solution::fits(anagram x);
	void aggregate_rec(vector<string>& v);
};