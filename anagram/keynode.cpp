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

void keynode::add(shared_ptr<keynode> child)
{
	int n_leafs = child->n_leafs;
	if (this->children.size() > 0)
		this->n_leafs += n_leafs;
	else
		this->n_leafs = n_leafs;

	int temp_height = child->max_height + 1;
	if (temp_height > this->max_height) {
		this->max_height = temp_height;
	}

	const string key = child->key;
	//! test: a duplicate node should not arrive here. it's already in child nodes
	//! delete this segment later
	auto stored_node_it = this->children.find(key);
	if (stored_node_it != this->children.end()) {
		shared_ptr<keynode> stored_node = stored_node_it->second;
		if (stored_node == child) {
			int a = 0; //break here
		}
	}

	if (!keyInDescendants(this->key, child)) {
		this->children[key] = child;
	}
	else {
		// at this point this node's key is found in the child's node, meaning this node is already a child of the child node
		// this is not supposed to happen. it likely happens with duplicate letters
		int a = 0; // break here
	}
}

void keynode::addToAllLeaves(shared_ptr<keynode> child)
{
	if (this->isLeaf() && this->key != child->key) {
		this->add(child);
	}
	else {
		for (auto it = this->children.begin(); it != this->children.end(); it++) {
			if (this->key != child->key)
				it->second->addToAllLeaves(child);
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
	return i_arr + 1;
}

bool keynode::keyInDescendants(string key, shared_ptr<keynode> node)
{
	if (key == "")
		return false;				// exception for the root node

	for (auto it = node->children.begin(); it != node->children.end(); it++) {
		if (isSubsequence(key, it->second->key)) {
			if (keyInDescendants(key, it->second))
				return true;
		}
	}
	return false;
}

bool keynode::isSubsequence(string& subseq, string& seq)
{
	// assume that both subseq and seq are sorted alphabetically
	string intersection = "";
	for (char c : seq) {
		size_t i = subseq.find(c);
		if (i != string::npos)
			intersection += subseq[i];
	}
	if (subseq.compare(intersection) == 0)
		return true;
	else
		return false;
}
