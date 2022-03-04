#pragma once

#include <vector>
#include <string>

using namespace std;

class keynode
{
private:
	string key;
	vector<keynode> children;
	int depth;

public:
	keynode(string key, int depth);
	void add(keynode node);
	string getKey();
	vector<vector<string>> traverseAll();
};

