#pragma once

#include <vector>
#include <string>

using namespace std;

class keynode
{
private:
	string key;
	vector<keynode>* children;

public:
	int depth;
	int n_leafs;

	string getKey();
	vector<keynode> getChildren();
	bool isLeaf();

	keynode();
	keynode(string key, int depth);

	void add(keynode& node);
	int traversePerNode(vector<vector<string>>* const& arr_ptr, vector<string> key_seq, int i, int j=0);
};

