#pragma once

#include <vector>
#include <string>

#include "keynode.h"

using namespace std;

class keytree
{
public:
	shared_ptr<keynode> root;
	int max_depth;

	keytree();
	shared_ptr<keynode> addKey(string key, shared_ptr<keynode> parent);
	void addChild(shared_ptr<keynode> child, shared_ptr<keynode> parent);
	vector<vector<string>> traverse() const;
};
