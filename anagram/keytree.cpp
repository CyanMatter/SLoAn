#include "keytree.h"

keytree::keytree()
{
	this->root = make_shared<keynode>(keynode("", 0));
}

shared_ptr<keynode> keytree::addKey(const string key, shared_ptr<keynode> parent_ptr)
{
	int child_depth = parent_ptr->depth + 1;
	shared_ptr<keynode> child_ptr = make_shared<keynode>(keynode(key, child_depth));
	parent_ptr->add(child_ptr);
	return child_ptr;
}

vector<vector<string>> keytree::traverse() const
{
	vector<vector<string>> arr(this->root->n_leafs);
	vector<string> seq(this->root->max_height);

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

pair<vector<shared_ptr<keynode>>, vector<shared_ptr<keynode>>> keytree::bestParents(vector<shared_ptr<keynode>> nodes_x, vector<shared_ptr<keynode>> nodes_y)
{
	int sum_leafs = 0, sum_max_height = 0;
	for (shared_ptr<keynode> child : nodes_x) {
		sum_leafs += child->n_leafs;
		sum_max_height += child->max_height;
	}
	for (shared_ptr<keynode> child : nodes_y) {
		sum_leafs -= child->n_leafs;
		sum_max_height -= child->max_height;
	}

	if (sum_leafs < 0) {	// sum of leaves in nodes_x is less than that in nodes_y
		return pair<vector<shared_ptr<keynode>>, vector<shared_ptr<keynode>>>(nodes_x, nodes_y);
	}
	else if (sum_leafs > 0) { // sum of leaves in nodes_x is greater than that in nodes_y
		return pair<vector<shared_ptr<keynode>>, vector<shared_ptr<keynode>>>(nodes_y, nodes_x);
	}// sum of leaves in nodes_x is equal to that in nodes_y
	else if (sum_max_height <= 0) { // and sum of max_height in nodes_ x is less than or equal to that in nodes_y
		return pair<vector<shared_ptr<keynode>>, vector<shared_ptr<keynode>>>(nodes_x, nodes_y);
	}
	else {	// sum of max_height in nodes_ x is greater than that in nodes_y
		return pair<vector<shared_ptr<keynode>>, vector<shared_ptr<keynode>>>(nodes_y, nodes_x);
	}
}
