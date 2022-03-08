#include "keynode.h"

keynode::keynode(string key, int depth)
{
	this->key = key;
	this->children = new vector<keynode>();
	this->leaf = true;
	this->depth = depth;
}

int keynode::getDepth()
{
	return this->depth;
}

int keynode::add(keynode node)
{
	this->children->push_back(node);

	if (this->leaf) {
		this->leaf = false;
		return 0;				// return 0: 1 leaf node was added but also 1 subtracted to the tree
	}
	else
		return 1;				// return 1: 1 leaf node was added
}

string keynode::getKey()
{
	return this->key;
}
vector<keynode> keynode::getChildren()
{
	return *this->children;
}

bool keynode::isLeaf()
{
	return this->leaf;
}

void keynode::traversePerNode(vector<vector<string>>* const& arr_ptr, vector<string> key_seq, int index)
{
	key_seq[index] = this->key;
	if (this->leaf) {
		(*arr_ptr)[index] = key_seq;
	}
	else {
		if (this->children->size() == 0)
			this->children->at(0).traversePerNode(arr_ptr, key_seq, index+1);
		else {
			for (keynode child : *this->children) {
				vector<string> key_seq_copy = key_seq;
				child.traversePerNode(arr_ptr, key_seq_copy, index+1);
			}
		}
	}
}
