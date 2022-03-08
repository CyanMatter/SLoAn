#pragma once

#include <vector>
#include <string>

using namespace std;

class keynode
{
private:
	string key;
	vector<keynode>* children;
	bool leaf;
	int depth;

public:
	string getKey();
	vector<keynode> getChildren();
	bool isLeaf();
	int getDepth();

	keynode(string key, int depth);
	int add(keynode node);
	void traversePerNode(vector<vector<string>>* const& arr_ptr, vector<string> key_seq, int index);
};

