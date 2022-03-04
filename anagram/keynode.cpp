#include "keynode.h"

keynode::keynode(string key, int depth)
{
	this->key = key;
	this->depth = depth;
}

void keynode::add(keynode node)
{
	this->children.push_back(node);
}

string keynode::getKey()
{
	return this->key;
}

vector<vector<string>> keynode::traverseAll()
{
	//TODO
}
