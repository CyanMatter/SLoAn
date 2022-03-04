#include "keytree.h"

keytree::keytree()
{
	this->root = new keynode("", 0);
	this->max_depth = 0;
	this->n_leafs = 0;
}

vector<vector<string>> keytree::traverse()
{
	vector<vector<string>> key_seqs(this->n_leafs);
	return this->root.traverseAll();
}
