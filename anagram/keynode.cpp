#include "keynode.h"

keynode::keynode()
{
	this->key = "";
	this->children = {};
	this->depth = 0;
	this->max_height = 0;
	this->n_leafs = 1;
}

keynode::keynode(const string key, const int depth)
{
	this->key = key;
	this->children = {};
	this->depth = depth;
	this->max_height = 0;
	this->n_leafs = 1;
}

bool keynode::operator==(const keynode& rhs) const noexcept
{
	return this->key == rhs.key
		&& this->children == rhs.children
		&& this->depth == rhs.depth
		&& this->max_height == rhs.max_height
		&& this->n_leafs == rhs.n_leafs;
}

bool keynode::isLeaf()
{
	return this->max_height == 0;
}

bool keynode::keyInDescendants(string key)
{
	for (auto it = this->children.begin(); it != this->children.end(); it++) {
		if (it->second->keyInDescendants(key))
			return true;
	}
	return false;
}


void keynode::add(shared_ptr<keynode> node)
{
	int n_leafs = node->n_leafs;
	if (this->children.size() > 0)
		this->n_leafs += n_leafs;
	else
		this->n_leafs = n_leafs;

	int temp_height = node->max_height + 1;
	if (temp_height > this->max_height) {
		this->max_height = temp_height;
	}

	const string key = node->key;
	//! test: a duplicate node should not arrive here. it's already in child nodes
	//! delete this segment later
	auto stored_node_it = this->children.find(key);
	if (stored_node_it != this->children.end()) {
		shared_ptr<keynode> stored_node = stored_node_it->second;
		if (stored_node == node) {
			int a = 0; //break here
		}
	}
	
	this->children[key] = node;
}

void keynode::addToAllLeaves(shared_ptr<keynode> node)
{
	if (this->isLeaf()) {
		this->add(node);
	}
	else {
		for (auto it = this->children.begin(); it != this->children.end(); it++) {
			it->second->addToAllLeaves(node);
		}
	}
}

int keynode::traversePerNode(vector<vector<string>>* const& arr_ptr, vector<string> seq, int i_arr, int i_seq) const
{
	seq[i_seq] = this->key;
	if (this->children.size() == 0) {
		(*arr_ptr)[i_arr] = seq;
	}
	else {
		for (auto it = children.begin(); it != children.end(); it++) {
			keynode child = (*it->second);
			vector<string> key_seq_copy = seq;
			i_arr = child.traversePerNode(arr_ptr, key_seq_copy, i_arr, i_seq + 1);
		}
		return i_arr;
	}
	return i_arr+1;
}
