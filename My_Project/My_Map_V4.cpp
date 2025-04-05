#include <iostream>
#include <memory>
#include <cassert>

#define red false
#define black true


void test();
//v4 Black-Red tree
//We need to make sure that all the insertion node should be red
//but if the tree is empty, after inserting we need to covert the node to black, since all the root should be black
//no constant red node, but black is not limited
//from each node to its leaf(null) node, the numbers of the black node should be same



template<typename T>
struct TreeNode
{
	T key; //the value
	bool color;
	TreeNode<T>* parent_node;
	TreeNode<T>* left_node;
	TreeNode<T>* right_node;


	TreeNode(T value) :
		color(red), key(value), parent_node(nullptr), left_node(nullptr), right_node(nullptr) 
	{
		//std::cout << "Constructing node..." << std::endl;
	}
	TreeNode(T value, TreeNode<T>* parent) :
		color(black), key(value), parent_node(parent), left_node(nullptr), right_node(nullptr)
	{ 
		//std::cout << "Constructing leaf node..." << std::endl;
	}
	~TreeNode()
	{
		//std::cout << "Destructing node..." << std::endl;
	}
};

template<typename T>
class MyRBT
{
public:

	MyRBT() :root(nullptr)
	{
		//std::cout << "Constructing tree..." << std::endl;
	}
	~MyRBT()
	{
		//std::cout << "Destructing tree..." << std::endl;
	}

	void insert(T key)
	{
		root = insert_node(root, key); //each node is the root for their kids
		
	}

	void remove(T key)
	{
		root = remove_node(root, key);
		
	}

	bool search(T key)
	{
		return search_node(root, key);
	}

	void print();

private:

	TreeNode<T>* root;

	TreeNode<T>* insert_node(TreeNode<T>*, T);
	TreeNode<T>* remove_node(TreeNode<T>*, T);
	TreeNode<T>* get_min(TreeNode<T>*);
	bool search_node(TreeNode<T>*, T);
	void inorder(TreeNode<T>*); //for print

	void left_rotate(TreeNode<T>*);
	void right_rotate(TreeNode<T>*);
	void balance_insert(TreeNode<T>*);
	void balance_remove(TreeNode<T>*);

};

template<typename T>
void MyRBT<T>::balance_insert(TreeNode<T>* node)
{

	if (node == root || node->parent_node->color == black)
	{
		//std::cout << "Finsih balancing..." << std::endl;
		node->color = black;
		return;
	}

	TreeNode<T>* grandparent = node->parent_node->parent_node;
	TreeNode<T>* uncle = nullptr;


	if (node->parent_node->key < grandparent->key)
	{
		//std::cout << "Start balancing left side..." << std::endl;
		uncle = grandparent->right_node;
		if (uncle != nullptr && uncle->color == red)
		{
			node->parent_node->color = black;
			grandparent->color = red;
			uncle->color = black;
			balance_insert(grandparent);
		}
		else
		{
			if (node->key > node->parent_node->key)
			{
				left_rotate(node->parent_node);
				balance_insert(node->parent_node);
			}
			else if (node->key < node->parent_node->key)
			{
				node->parent_node->color = black;
				grandparent->color = red;
				right_rotate(grandparent);
			}
		}

	}
	else
	{
		//std::cout << "Start balancing right side..." << std::endl;
		uncle = grandparent->left_node;
		if (uncle != nullptr && uncle->color == red)
		{
			node->parent_node->color = black;
			grandparent->color = red;
			uncle->color = black;
			balance_insert(grandparent);
		}
		else
		{
			if (node->key < node->parent_node->key)
			{
				right_rotate(node->parent_node);
				balance_insert(node->parent_node);
			}
			else if (node->key > node->parent_node->key)
			{
				node->parent_node->color = black;
				grandparent->color = red;
				left_rotate(grandparent);
			}
		}
	}

	root->color = black;
	return;
}

template<typename T>
void MyRBT<T>::left_rotate(TreeNode<T>* node)
{
	if (root == node)
	{
		node->parent_node = node->right_node;
		node->right_node = node->right_node->left_node;
	}
	else
	{
		auto temp_parent = node->parent_node;
		node->parent_node = node->right_node;
		node->right_node = node->right_node->left_node;
		node->parent_node->parent_node = temp_parent;
	}
}

template<typename T>
void MyRBT<T>::right_rotate(TreeNode<T>* node)
{
	if (root == node)
	{
		node->parent_node = node->left_node;
		node->left_node = node->left_node->right_node;
	}
	else
	{
		auto temp_parent = node->parent_node;
		node->parent_node = node->left_node;
		node->left_node = node->left_node->right_node;
		node->parent_node->parent_node = temp_parent;
	}
}

template<typename T>
TreeNode<T>* MyRBT<T>::insert_node(TreeNode<T>* node, T key) //recursion insert
{
	if (root == nullptr) //if there are no nodes, means that this tree is empty or we already find the empty place to create the node,
		//and the root node should be black
	{
		node = new TreeNode<T>(key);
		node->color = black;
		return node;
	}
	if (!node)
		return new TreeNode<T>(key);

	if (key < node->key)
	{
		node->left_node = insert_node(node->left_node, key);
		node->left_node->parent_node = node;
		if (node->left_node->parent_node->color == black)
			return node;
		else
			balance_insert(node->left_node);



	}
	else if (key > node->key)
	{
		node->right_node = insert_node(node->right_node, key);
		node->right_node->parent_node = node;
		if (node->right_node->parent_node->color == black)
			return node;
		else
			balance_insert(node->right_node);

	}
	else
		return nullptr;

	return node;
}

template<typename T>
TreeNode<T>* MyRBT<T>::get_min(TreeNode<T>* node)
{
	if (node->left_node)
		node = get_min(node->left_node);

	return node;
}

template<typename T>
void MyRBT<T>::balance_remove(TreeNode<T>* node)
{
	TreeNode<T>* brother;
	
	if (node->color == red)
	{
		node->color = black;
		return;
	}
	else
	{
		brother = node->parent_node->right_node;
		if (brother->color == red)
		{
			brother->color = black;
			node->parent_node->color = red;
			left_rotate(node->parent_node);
			balance_remove(node);
		}
		else if ((brother->color && brother->left_node && brother->right_node) == black)
		{
			brother->color = red;
			balance_remove(node->parent_node);
		}
		else if ((brother->color && brother->right_node->color) == black && brother->left_node->color == red)
		{
			brother->color = red;
			brother->left_node->color = black;
			right_rotate(brother);
			balance_remove(node);
		}
		else if ((brother->color && brother->left_node->color) == black && brother->right_node->color == red)
		{
			brother->color = node->parent_node->color;
			node->parent_node->color = black;
			brother->right_node->color = black;
			left_rotate(node->parent_node);
		}
	}

}

template<typename T>
TreeNode<T>* MyRBT<T>::remove_node(TreeNode<T>* node, T key)
{
	assert(root && "Error: Attempting to remove an empty tree!");

	if (key < node->key)
		node->left_node = remove_node(node->left_node, key);
	else if (key > node->key)
		node->right_node = remove_node(node->right_node, key);
	else if (key == node->key)
	{
	
		

		if (node->color == red) //only root node or no child node
		{
			if (!node->left_node && !node->right_node)
			
			{
				delete node;
				return nullptr;
			}
			
			else if (!node->left_node && node->right_node)
			{
				auto temp = node;
				auto temp_parent = node->parent_node;
				node = node->right_node;
				node->parent_node = temp_parent;
				delete temp;
				return node;
			}
			else if (node->left_node && !node->right_node)
			{
				auto temp = node;
				auto temp_parent = node->parent_node;
				node = node->left_node;
				node->parent_node = temp_parent;
				delete temp;
				return node;
			}
			else
			{
				auto temp = get_min(node->right_node);
				node->key = temp->key;
				node->right_node = remove_node(node->right_node, temp->key);
			}
		}

		else
		{
			if (!node->left_node && !node->right_node && node == root)
			{
				delete root;
				return nullptr;
			}

			if (!node->left_node && !node->right_node)
			{
				auto temp_parent = node->parent_node;
				TreeNode<T>* leaf_node = new TreeNode<T>(T(), temp_parent);
				delete node;
				balance_remove(leaf_node);
				delete leaf_node;
				return nullptr;
			}
			else if (!node->left_node && node->right_node)
			{
				auto temp = node;
				auto temp_parent = node->parent_node;
				node = node->right_node;
				node->parent_node = temp_parent;
				delete temp;
				balance_remove(node);
				return node;
			}
			else if (node->left_node && !node->right_node)
			{
				auto temp = node;
				auto temp_parent = node->parent_node;
				node = node->left_node;
				node->parent_node = temp_parent;
				delete temp;
				balance_remove(node);
				return node;
			}
			else
			{
				auto temp = get_min(node->right_node);
				node->key = temp->key;
				node->right_node = remove_node(node->right_node, temp->key);
			}
		}
		
	}

	else
	{
		std::cout << "No node: " << key << " in this tree." << std::endl;
	}
	

;

	
	return node;

}


template<typename T>
bool MyRBT<T>::search_node(TreeNode<T>* node, T key)
{
	assert(root && "Error: Attempting to search an empty tree!");
	if (!node)
		return false;
	if (key == node->key)
		return true;

	return (key < node->key) ? search_node(node->left_node, key) : search_node(node->right_node, key);
}



template<typename T>
void MyRBT<T>::inorder(TreeNode<T>* node)
{
	if (!node)
		return;
	inorder(node->left_node);
	if (node == root)
		std::cout << "\n***The root is: " << node->key << "(" << ((node->color) ? "B)" : "R)") << "***" << std::endl;
	else
	{
		std::cout << node->key << "(" << ((node->color) ? "B)" : "R)") << std::flush;
		//std::cout << "-->Parent-->" << node->parent_node->key << "(" << ((node->parent_node->color) ? "B) " : "R) ") << std::flush;
	}
	inorder(node->right_node);
	

}


template<typename T>
void MyRBT<T>::print()
{
	assert(root && "Error: Attempting to print an empty tree!");
	std::cout << "The tree:\n" << std::flush;
	inorder(root);
	std::cout << std::endl;
}

void test()
{
	MyRBT<int> tree;

	
	int values[] = { 10, 5, 15, 2, 7, 12, 18, 1, 3, 6, 8 };
	for (int val : values) {
		std::cout << "\nInserting " << val << "...\n";
		tree.insert(val);
		tree.print();
	}


}


int main()
{
	test();
	


	return 0;
}
