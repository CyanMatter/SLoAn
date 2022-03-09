#include "keynode.h"

keynode::keynode()
{
	this->key = "";
	this->children = new vector<keynode>();
	this->depth = 0;
	this->n_leafs = 1;
}

keynode::keynode(string key, int depth)
{
	this->key = key;
	this->children = new vector<keynode>();
	this->depth = depth;
	this->n_leafs = 1;
}

void keynode::add(keynode& node)
{
	if (this->children->size() > 0)
		this->n_leafs += node.n_leafs;
	else if (node.n_leafs > 1)
		this->n_leafs = node.n_leafs;

	this->children->push_back(node);
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
	return this->children->size() == 0;
}

int keynode::traversePerNode(vector<vector<string>>* const& arr_ptr, vector<string> seq, int i_arr, int i_seq)
{
	seq[i_seq] = this->key;
	if (this->isLeaf()) {
		(*arr_ptr)[i_arr] = seq;
	}
	else {
		for (keynode child : *this->children) {
			vector<string> key_seq_copy = seq;
			i_arr = child.traversePerNode(arr_ptr, key_seq_copy, i_arr, i_seq+1);
		}
		return i_arr;
	}
	return i_arr+1;
}
