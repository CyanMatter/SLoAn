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

	static pair<vector<shared_ptr<keynode>>, vector<shared_ptr<keynode>>> bestParents(vector<shared_ptr<keynode>> nodes_x, vector<shared_ptr<keynode>> nodes_y);
};
