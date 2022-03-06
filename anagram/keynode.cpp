#include "keynode.h"

keynode::keynode(string key, int depth)
{
	this->key = key;
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

void keynode::traversePerNode(vector<vector<string>>* const& arr_ptr, vector<string> key_seq)
{
	key_seq.push_back(this->key);

	if (this->leaf) {
		arr_ptr->push_back(key_seq);
	}
	else {
		if (this->children->size() == 0)
			this->children->at(0).traversePerNode(arr_ptr, key_seq);
		else {
			for (keynode child : *this->children) {
				vector<string> key_seq_copy = key_seq;
				child.traversePerNode(arr_ptr, key_seq_copy);
			}
		}
	}
}
