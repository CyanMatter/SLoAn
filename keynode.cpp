#include "keynode.h"

keynode::keynode()
{
	this->key = "";
	this->children = {};
	this->parents = {};
	this->max_height = 1;
	this->n_leaves = 1;
}

keynode::keynode(const string& key)
{
	this->key = key;
	this->children = {};
	this->parents = {};
	this->max_height = 1;
	this->n_leaves = 1;
}

keynode::keynode(const string& key, const unordered_map<string, shared_ptr<keynode>> children, const unordered_map<string, shared_ptr<keynode>> parents, const int max_height, const int n_leaves)
{
	this->key = key;
	this->children = children;
	this->parents = parents;
	this->max_height = max_height;
	this->n_leaves = n_leaves;
}

bool keynode::operator==(const keynode& rhs) const noexcept
{
	return this->key == rhs.key
		&& this->children == rhs.children
		&& this->parents == rhs.parents
		&& this->max_height == rhs.max_height
		&& this->n_leaves == rhs.n_leaves;
}

bool keynode::isLeaf()
{
	return this->max_height == 0;
}

bool keynode::childKnown(shared_ptr<keynode> child)
{
	auto it = this->children.find(child->key);
	while (it != this->children.end()) {
		if (child == it->second) {
			return true;
		}
		it++;
	}
	return false;
}

void keynode::updateNLeaves(const int n_leaves)
{
	if (this->children.size() > 0)
		this->n_leaves += n_leaves;
	else
		this->n_leaves = n_leaves;
}

void keynode::updateMaxHeight(const int height)
{
	if (height <= this->max_height) {
		return;
	}

	this->max_height = height;

	for (auto it = this->parents.begin(); it != this->parents.end(); it++) {
		it->second->updateMaxHeight(height + 1);
	}
}

void keynode::safelyAdd(shared_ptr<keynode> child, shared_ptr<keynode> parent)
{
	const string& key = child->key;

	//! test: a duplicate node should not arrive here. it's already in child nodes
	//! delete this segment later
	auto stored_node_it = parent->children.find(key);
	if (stored_node_it != parent->children.end()) {
		shared_ptr<keynode> stored_node = stored_node_it->second;
		assert(stored_node != child && "This keynode cannot already contain a node "
			"that is identical to the one that is being added.");
	}

	//!WIP parent and/or child can add a chain of nodes to each other.
	//! I believe each and every node in the chain should be tested in nodeInDescendants and nodeInAncestors.
	//! But consider that making deep copies of every added node might suffice.
	if (parent->key == "" || (!parent->nodeInDescendants(child) && !child->nodeInAncestors(parent))) {
		parent->children[key] = child;
		child->parents[parent->key] = parent;
	}
	else {
		throw std::exception("At this point this node's key is found in the child's node, "
			"meaning this node is already a child of the child node.This is not supposed "
			"to happen. It likely happens with duplicate letters");
	}
}

void keynode::add(shared_ptr<keynode> child, shared_ptr<keynode> parent)
{
	parent->updateNLeaves(child->n_leaves);
	parent->updateMaxHeight(child->max_height + 1);
	safelyAdd(child, parent);
}

void keynode::add(vector<shared_ptr<keynode>> children, shared_ptr<keynode> parent, const int sum_leaves, const int max_height_children)
{
	parent->updateNLeaves(sum_leaves);
	parent->updateMaxHeight(max_height_children + 1);
	
	for (shared_ptr<keynode> child : children) {
		safelyAdd(child, parent);
	}
}

void keynode::add(vector<shared_ptr<keynode>> children, shared_ptr<keynode> parent)
{
	const int sum_leaves = sumLeaves(children);
	const int max_height = maxHeight(children);
	keynode::add(children, parent, sum_leaves, max_height);
}

void keynode::addToAllLeaves(shared_ptr<keynode> child, shared_ptr<keynode> node)
{
	if (node->isLeaf() && node->key != child->key) {
		keynode::add(child, node);
	}
	else {
		for (auto it = node->children.begin(); it != node->children.end(); it++) {
			if (node->key != child->key)
				keynode::addToAllLeaves(child, it->second);
		}
	}
}

void keynode::addToAllLeaves(vector<shared_ptr<keynode>> children, shared_ptr<keynode> node)
{
	const int sum_leaves = sumLeaves(children);
	const int max_height = maxHeight(children);
	keynode::addToAllLeaves_rec(children, node, sum_leaves, max_height);
}

void keynode::addToAllLeaves(vector<shared_ptr<keynode>> children, shared_ptr<keynode> node, const int sum_leaves, const int max_height)
{
	keynode::addToAllLeaves_rec(children, node, sum_leaves, max_height);
}

int keynode::addToAllLeaves_rec(vector<shared_ptr<keynode>> children, shared_ptr<keynode> node, const int sum_leaves, const int max_height)
{
	if (node->isLeaf()) {
		keynode::add(children, node);
		node->max_height = max_height + 1;
		return node->max_height;
	}
	else {
		int height;
		for (auto it = node->children.begin(); it != node->children.end(); it++) {
			height = keynode:: addToAllLeaves_rec(children, it->second, sum_leaves, max_height);
			height++;
			if (node->max_height < height) {
				node->max_height = height;
			}
		}
		return node->max_height;
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
			shared_ptr<keynode> child = it->second;
			vector<string> key_seq_copy = seq;
			i_arr = child->traversePerNode(arr_ptr, key_seq_copy, i_arr, i_seq + 1);
		}
		return i_arr;
	}
	return i_arr + 1;
}

shared_ptr<keynode> keynode::deepCopy()
{
	unordered_map<string, shared_ptr<keynode>> copied_children = {};
	for (auto it_child = this->children.begin(); it_child != this->children.end(); it_child++) {
		shared_ptr<keynode> child = it_child->second;
		shared_ptr<keynode> copied_child = child->deepCopy();
		copied_children[copied_child->key] = copied_child;
	}
	keynode copied_node(this->key, copied_children, {}, this->max_height, this->n_leaves);
	return make_shared<keynode>(copied_node);
}

int keynode::sumLeaves(vector<shared_ptr<keynode>> nodes)
{
	int sum = 0;
	for (shared_ptr<keynode> node : nodes) {
		sum += node->n_leaves;
	}
	return sum;
}

int keynode::maxHeight(vector<shared_ptr<keynode>> nodes) {
	int max_height = 0;
	for (shared_ptr<keynode> node : nodes) {
		if (node->max_height > max_height)
			max_height = node->max_height;
	}
	return max_height;
}

bool keynode::nodeInAncestors(shared_ptr<keynode> node)
{
	{
		auto it = this->parents.find(node->key);
		if (it != this->parents.end() && it->second == node)
			return true;
	}
	for (auto it = this->parents.begin(); it != this->parents.end(); it++) {
		shared_ptr<keynode> parent = it->second;
		if (parent->nodeInAncestors(node))
			return true;
	}

	return false;
}

bool keynode::nodeInDescendants(shared_ptr<keynode> node)
{
	{
		auto it = this->children.find(node->key);
		if (it != this->children.end() && it->second == node)
			return true;
	}
	for (auto it = this->children.begin(); it != this->children.end(); it++) {
		shared_ptr<keynode> child = it->second;
		if (child->nodeInDescendants(node))
			return true;
	}

	return false;
}

bool keynode::keyInDescendants(const string& key)
{
	{
		auto it = this->children.find(key);
		if (it != this->children.end())
			return true;
	}
	for (auto it = this->children.begin(); it != this->children.end(); it++) {
		shared_ptr<keynode> child = it->second;
		if (child->keyInDescendants(key))
			return true;
	}

	return false;
}

bool keynode::isSubsequenceOfKey(const string& subseq)
{
	// assume that both subseq and this->key are sorted alphabetically
	string intersection = "";
	for (char c : this->key) {
		size_t i = subseq.find(c);
		if (i != string::npos)
			intersection += c;
	}
	return subseq.compare(intersection) == 0;
}
