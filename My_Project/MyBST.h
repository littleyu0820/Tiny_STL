#ifndef MYBST_H
#define MYBST_H


#include <iostream>
#include <memory>
#include <cassert>


template<typename T>
struct TreeNode
{
	T key; //the value
	TreeNode<T>* left_node;
	TreeNode<T>* right_node;


	TreeNode(T value) :key(value), left_node(nullptr), right_node(nullptr) {}
};

template<typename T>
class MyBst
{
public:

	MyBst() :root(nullptr) {}
	~MyBst() {}

	void insert(T key)
	{
		root = insert_node(root, key); //each node is the root for their kids
	}

	bool search(T key)
	{
		return search_node(root, key);
	}

	void print();

private:

	TreeNode<T>* root;

	TreeNode<T>* insert_node(TreeNode<T>*, T);
	bool search_node(TreeNode<T>*, T);
	void inorder(TreeNode<T>*); //for print

};

template<typename T>
TreeNode<T>* MyBst<T>::insert_node(TreeNode<T>* node, T key) //recursion insert
{
	if (!node) //if there are no nodes, means that this tree is empty or we already find the empty place to create the node
		return new TreeNode<T>(key);

	if (key < node->key) //left < right
		node->left_node = insert_node(node->left_node, key);

	else if (key > node->key) //rgiht
		node->right_node = insert_node(node->right_node, key);

	return node; //key == node->key ; 1 value 1 node, no repeat
}


template<typename T>
bool MyBst<T>::search_node(TreeNode<T>* node, T key)
{
	assert(root && "Error: Attempting to search an empty tree!");
	if (!node)
		return false;
	if (key == node->key)
		return true;

	return (key < node->key) ? search_node(node->left_node, key) : search_node(node->right_node, key);
}



template<typename T>
void MyBst<T>::inorder(TreeNode<T>* node)
{
	if (!node)
		return;
	inorder(node->left_node);
	std::cout << node->key << " " << std::flush;
	inorder(node->right_node);

}


template<typename T>
void MyBst<T>::print()
{
	assert(root && "Error: Attempting to print an empty tree!");
	std::cout << "The tree is: " << std::flush;
	inorder(root);
	std::cout << std::endl;
}


#endif
