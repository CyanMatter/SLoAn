#pragma once

#include <vector>
#include <string>

#include "keynode.h"

using namespace std;

class keytree
{
public:
	shared_ptr<keynode> root;

	keytree();
	shared_ptr<keynode> addKey(const string key, shared_ptr<keynode> parent);
	vector<vector<string>> traverse() const;
};
