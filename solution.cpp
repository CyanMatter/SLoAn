#include "solution.h"

solution::solution()
{
	// Used to construct an empty set solution
	this->is_empty = true;
}

solution::solution(const string& key, const bool is_anagram)
{
	// Used to construct a new solution
	this->is_empty = true;
	this->key = key;
	this->set = {};
	this->max_height = 1;
	this->leaves = 1;
	this->is_leaf = true;
	this->is_anagram = is_anagram;
}

solution::solution(const string& key, vector<anagram> set, const int max_height, const int leaves, const bool is_leaf, const bool is_anagram)
{
	// Used to construct a solution with anagrams
	this->is_empty = (set.size() == 0);
	this->key = key;
	this->set = set;
	this->max_height = max_height;
	this->leaves = leaves;
	this->is_leaf = is_leaf;
	this->is_leaf = is_anagram;
}

string solution::getKey() { return this->key; }
vector<anagram> solution::getSet() { return this->set; }
shared_ptr<solution> solution::getPtr() { return this->ptr; }

void solution::add(const anagram new_anagram)
{
	assert(this->fits(new_anagram));
	this->set.push_back(new_anagram);
}

vector<vector<string>> solution::aggregate()
{
	vector<vector<string>> anagram_arr;
	for (anagram a : this->set) {
		for (shared_ptr<solution> s : a) {
		}
	}
	return anagram_arr;
}

void solution::aggregate_rec(vector<string>& v)
{

}

/// <summary>
/// Concatenates all keys of solutions in anagram a. The resulting string is compared to this solution's key.
/// </summary>
/// <param name="a">The anagram used in the comparison.</param>
/// <returns>True if the concatenation of keys in the anagram equal this solution's key. False otherwise.</returns>
const bool solution::fits(anagram a)
{
	string seq = string();
	for (shared_ptr<solution> s : a) {
		seq += s->key;
	}
	sort(seq.begin(), seq.end());
	return seq == this->key;
}