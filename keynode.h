#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <cassert>

using std::string; using std::unordered_map;
using std::shared_ptr; using std::make_shared;
using std::vector;

class keynode
{
public:
	string key;
	unordered_map<string, shared_ptr<keynode>> children;
	unordered_map<string, shared_ptr<keynode>> parents;
	int max_height;
	int n_leaves;

	keynode();
	keynode(const string& key);
	keynode(const string& key, const unordered_map<string, shared_ptr<keynode>> children, const unordered_map<string, shared_ptr<keynode>> parents, const int max_height, const int n_leaves);

	bool operator==(const keynode& rhs) const noexcept;
	bool operator!=(const keynode& rhs) const noexcept;
	
	bool isLeaf();
	bool childKnown(shared_ptr<keynode> child);

	void updateNLeaves(const int n_leaves);
	void updateMaxHeight(const int height);
	bool nodeInRelatives(shared_ptr<keynode> node, unordered_map<string, shared_ptr<keynode>> relatives, unordered_map<string, shared_ptr<keynode>>(*func)(shared_ptr<keynode>));
	bool nodeInAncestors(shared_ptr<keynode> node);
	bool nodeInDescendants(shared_ptr<keynode> node);
	bool keyInDescendants(const string& key);
	bool isSubsequenceOfKey(const string& subseq);
	int traversePerNode(vector<vector<string>>* const& arr_ptr, vector<string> key_seq, int i, int j=0) const;
	keynode deepCopy();

	static void safelyAdd(shared_ptr<keynode> child, shared_ptr<keynode> parent);
	static void safelyAdd(unordered_map<string, unordered_map<string, shared_ptr<keynode>>>::const_iterator it_children, shared_ptr<keynode> parent);
	static void add(shared_ptr<keynode> child, shared_ptr<keynode> parent);
	static void add(unordered_map<string, unordered_map<string, shared_ptr<keynode>>>::const_iterator it_children, shared_ptr<keynode> parent);
	static void addToAllLeaves(shared_ptr<keynode> child, shared_ptr<keynode> node);
	static void addToAllLeaves(unordered_map<string, shared_ptr<keynode>> children, shared_ptr<keynode> node);
	static void addToAllLeaves(unordered_map<string, shared_ptr<keynode>> children, shared_ptr<keynode> node, const int sum_leaves, const int max_height);
	static int sumLeaves(unordered_map<string, shared_ptr<keynode>> nodes);
	static int maxHeight(unordered_map<string, shared_ptr<keynode>> nodes);

private:
	static unordered_map<string, shared_ptr<keynode>> getChildren(shared_ptr<keynode> node);
	static unordered_map<string, shared_ptr<keynode>> getParents(shared_ptr<keynode> node);
	static void add(unordered_map<string, unordered_map<string, shared_ptr<keynode>>>::const_iterator it_children, shared_ptr<keynode> parent, const int sum_leaves, const int max_height_children);
	static int addToAllLeaves_rec(unordered_map<string, shared_ptr<keynode>> children, shared_ptr<keynode> node, const int sum_leaves, const int max_height);
};