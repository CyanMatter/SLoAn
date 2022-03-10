#pragma once

#include <unordered_map>
#include <string>

using namespace std;

class keynode
{
private:
	string key;
	unordered_map<string, keynode> children;

public:
	int depth;
	int n_leafs;

	string getKey();
	unordered_map<string, keynode> getChildren();
	unordered_map<string, keynode>::const_iterator childWithKey(string key);
	bool isLeaf();

	keynode();
	keynode(string key, int depth);

	bool operator==(const keynode& rhs) const noexcept;

	void add(keynode& node);
	int traversePerNode(vector<vector<string>>* const& arr_ptr, vector<string> key_seq, int i, int j=0);
};

