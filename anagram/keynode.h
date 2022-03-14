#pragma once

#include <unordered_map>
#include <string>
#include <memory>

using namespace std;

class keynode
{
public:
	string key;
	unordered_map<string, shared_ptr<keynode>> children;
	int depth;
	int max_height;
	int n_leafs;

	keynode();
	keynode(const string key, const int depth);

	bool operator==(const keynode& rhs) const noexcept;

	bool isLeaf();
	
	void add(shared_ptr<keynode> node);
	void addToAllLeaves(shared_ptr<keynode> child);
	int traversePerNode(vector<vector<string>>* const& arr_ptr, vector<string> key_seq, int i, int j=0) const;

	static bool keyInDescendants(string key, shared_ptr<keynode> node);
	static bool isSubsequence(string& x, string& y);
};
