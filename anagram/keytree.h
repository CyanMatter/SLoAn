#pragma once

#include <vector>
#include <string>

#include "keynode.h"

using namespace std;

class keytree
{
public:
	keynode root;
	int max_depth;

	keytree();
	keynode addKey(string key, keynode& parent);
	void addChild(keynode child, keynode& parent);
	vector<vector<string>> traverse();
};

