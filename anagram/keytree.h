#pragma once

#include <vector>
#include <string>

#include "keynode.h"

using namespace std;

class keytree
{
public:
	keynode* root;
	int max_depth;
	int n_leafs;

	keytree();
	keynode addKey(string key, keynode parent);
	vector<vector<string>> traverse();
};

