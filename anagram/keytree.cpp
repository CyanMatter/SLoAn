#include "keytree.h"

keytree::keytree()
{
	this->root = make_shared<keynode>(keynode("", 0));
	this->max_depth = 0;
}

shared_ptr<keynode> keytree::addKey(string key, shared_ptr<keynode> parent)
{
	int child_depth = parent->depth + 1;
	keynode child = keynode(key, child_depth);
	shared_ptr<keynode> child_ptr = make_shared<keynode>(child);
	this->addChild(child_ptr, parent);
	return child_ptr;
}

void keytree::addChild(shared_ptr<keynode> child, shared_ptr<keynode> parent)
{
	parent->add(child);
	if (child->depth > this->max_depth)
		this->max_depth = child->depth;
}

vector<vector<string>> keytree::traverse() const
{
	vector<vector<string>> arr(this->root->n_leafs);
	vector<string> seq(this->max_depth);

	unordered_map<string, shared_ptr<keynode>> children = this->root->children;
	unordered_map<string, shared_ptr<keynode>>::const_iterator it_child = children.begin();
	int i_arr = 0;

	while (i_arr < this->root->n_leafs && it_child != children.end()) {
		const keynode child = *(it_child->second);
		i_arr = child.traversePerNode(&arr, seq, i_arr);
		it_child++;
	}
	return arr;
}
