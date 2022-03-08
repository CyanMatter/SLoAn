#include "keytree.h"

keytree::keytree()
{
	this->root = new keynode("", 0);
	this->max_depth = 0;
	this->max_anagrams = 0;
	this->n_leafs = 1;
}

keynode keytree::addKey(string key, keynode* const& parent)
{
	int child_depth = parent->getDepth() + 1;
	keynode child = keynode(key, child_depth);
	this->n_leafs += parent->add(child);
	
	if (child_depth > this->max_depth)
		this->max_depth = child_depth;
	
	return child;
}

vector<vector<string>> keytree::traverse()
{
	vector<vector<string>> out(this->n_leafs);
	vector<string> key_seq(this->max_depth);

	for (int i = 0; i < this->n_leafs; i++) {
		keynode child = root->getChildren()[i];
		child.traversePerNode(&out, key_seq, i);
	}
	return out;
}
