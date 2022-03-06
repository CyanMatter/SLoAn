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
	int getDepth();

	keynode(string key, int depth);
	int add(keynode node);
	string getKey();
	void traversePerNode(vector<vector<string>>* const& arr_ptr, vector<string> key_seq);
};

