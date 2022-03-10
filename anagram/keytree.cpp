#include "keytree.h"

keytree::keytree()
{
	this->root = keynode("", 0);
	this->max_depth = 0;
}

keynode keytree::addKey(string key, keynode& parent)
{
	int child_depth = parent.depth + 1;
	keynode child = keynode(key, child_depth);
	this->addChild(child, parent);
	return child;
}

void keytree::addChild(keynode child, keynode& parent)
{
	parent.add(child);
	if (child.depth > this->max_depth)
		this->max_depth = child.depth;
}

vector<vector<string>> keytree::traverse()
{
	vector<vector<string>> arr(this->root.n_leafs);
	vector<string> seq(this->max_depth);

	unordered_map<string, keynode> children = this->root.getChildren();
	unordered_map<string, keynode>::const_iterator it_child = children.begin();
	int i_arr = 0;

	while (i_arr < this->root.n_leafs && it_child != children.end()) {
		keynode child = it_child->second;
		i_arr = child.traversePerNode(&arr, seq, i_arr);
		it_child++;
	}
	return arr;
}
