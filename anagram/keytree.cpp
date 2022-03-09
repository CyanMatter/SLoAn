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

	for (int i_child = 0, i_arr = 0; i_arr < this->root.n_leafs && i_child < this->root.getChildren().size(); i_child++) {
		keynode child = this->root.getChildren()[i_child];
		i_arr = child.traversePerNode(&arr, seq, i_arr);
	}
	return arr;
}
