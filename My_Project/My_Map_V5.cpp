#include <iostream>
#include <memory>
#include <cassert>

#define red false
#define black true


void test();
//v5 Black-Red tree
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
		leaf_node = new TreeNode<T>(T());
		leaf_node->color = black;
		leaf_node->left_node = leaf_node->right_node = leaf_node->parent_node = nullptr;
		//std::cout << "Constructing tree..." << std::endl;
	}
	~MyRBT()
	{
		//std::cout << "Destructing tree..." << std::endl;
	}

	void insert(T key)
	{
		root = insert_node(root, key);
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
	TreeNode<T>* leaf_node;

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
TreeNode<T>* MyRBT<T>::insert_node(TreeNode<T>* cur_node, T key)
{
	TreeNode<T>* new_node = new TreeNode<T>(key);
	TreeNode<T>* last_node = nullptr;

	while (cur_node != nullptr)
	{
		last_node = cur_node;
		assert(key != last_node->key && "Error: Attempting to insert an exist element!");
		if (key < cur_node->key)
			cur_node = cur_node->left_node;
		else
			cur_node = cur_node->right_node;
	}

	new_node->parent_node = last_node;

	if (last_node == nullptr)
		root = new_node;
	else if (key < last_node->key)
		last_node->left_node = new_node;
	else
		last_node->right_node = new_node;


	new_node->left_node = nullptr;
	new_node->right_node = nullptr;

	balance_insert(new_node);
	return root;
}
template<typename T>
void MyRBT<T>::balance_insert(TreeNode<T>* node)
{
	
	if (node == root)
	{
		//std::cout << "Finsih balancing..." << std::endl;
		node->color = black;
		return;
	}
	if (node->parent_node->color == black)
	{
		return;
	}

	TreeNode<T>* grandparent = node->parent_node->parent_node;
	TreeNode<T>* uncle = nullptr;


	if (node->parent_node->key < grandparent->key)
	{
		std::cout << "Start balancing left side..." << std::endl;
		uncle = grandparent->right_node;
		if (uncle != nullptr && uncle->color == red)
		{
			std::cout << "Uncle is: " << uncle->key << std::endl;
			node->parent_node->color = black;
			grandparent->color = red;
			uncle->color = black;
			balance_insert(grandparent);
		}
		else
		{
			
			if (node->key > node->parent_node->key)
			{
				
				node = node->parent_node;
				left_rotate(node);
				grandparent = node->parent_node->parent_node;
			}
			
			
			node->parent_node->color = black;
			grandparent->color = red;
			right_rotate(grandparent);
			
		}


	}
	else
	{
		std::cout << "Start balancing right side..." << std::endl;
		uncle = grandparent->left_node;
		if (uncle != nullptr && uncle->color == red)
		{
			std::cout << "Uncle is: " << uncle->key << std::endl;
			node->parent_node->color = black;
			grandparent->color = red;
			uncle->color = black;
			balance_insert(grandparent);
		}
		else
		{	
			if (node->key < node->parent_node->key)
			{
	
				node = node->parent_node;
				right_rotate(node);
				grandparent = node->parent_node->parent_node;
				
			}
		
			node->parent_node->color = black;
			grandparent->color = red;
			left_rotate(grandparent);
		}

	}


	root->color = black;
	
}

template<typename T>
void MyRBT<T>::left_rotate(TreeNode<T>* node) 
{
	if (node == nullptr || node->right_node == nullptr)
		return;
	TreeNode<T>* temp = node->right_node;
	node->right_node = temp->left_node;
	if (temp->left_node != nullptr)
		temp->left_node->parent_node = node;

	temp->parent_node = node->parent_node;

	if (node->parent_node == nullptr)
		root = temp;
	else if (node == node->parent_node->left_node)
		node->parent_node->left_node = temp;
	else
		node->parent_node->right_node = temp;

	temp->left_node = node;
	node->parent_node = temp;
}

template<typename T>
void MyRBT<T>::right_rotate(TreeNode<T>* node)
{
	if (node == nullptr || node->left_node == nullptr)
		return;
	TreeNode<T>* temp = node->left_node;
	node->left_node = temp->right_node;
	if (temp->right_node != nullptr)
		temp->right_node->parent_node = node;

	temp->parent_node = node->parent_node;

	if (node->parent_node == nullptr)
		root = temp;
	else if (node == node->parent_node->right_node)
		node->parent_node->right_node = temp;
	else
		node->parent_node->left_node = temp;

	temp->right_node = node;
	node->parent_node = temp;
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

	
	if (node == root)
		return;
	
	TreeNode<T>* brother;

	if (node->key < node->parent_node->key)
	{
		brother = node->parent_node->right_node;
		if (brother->right_node == nullptr)
			brother->right_node = leaf_node;
		

		if(brother->left_node == nullptr)
			brother->left_node = leaf_node;

		if (brother != nullptr && brother->color == red)
		{
			brother->color = black;
			node->parent_node->color = red;
			left_rotate(node->parent_node);
			brother = node->parent_node->right_node;
			balance_remove(brother);
		}
		else if(brother != nullptr && brother->color == black)
		{
			if (brother->right_node->color == black && brother->left_node->color == black)
			{
				brother->color = red;
				if (node->parent_node->color == red)
				{
					node->parent_node->color = black;
					return;
				}
				else
					balance_remove(node->parent_node);
			}
			else if (brother->right_node->color == black && brother->left_node->color == red)
			{
				brother->color = red;
				brother->left_node->color = black;
				right_rotate(brother);
				brother = node->parent_node->right_node;
				balance_remove(brother);
			}
			else if (brother->right_node->color == red)
			{
				brother->color = node->parent_node->color;
				node->parent_node->color = black;
				brother->right_node->color = black;
				left_rotate(node->parent_node);
			}
		}
	}
	else
	{
		brother = node->parent_node->left_node;
		if (brother->right_node == nullptr)
			brother->right_node = leaf_node;


		if (brother->left_node == nullptr)
			brother->left_node = leaf_node;

		if (brother != nullptr && brother->color == red)
		{
			brother->color = black;
			node->parent_node->color = red;
			right_rotate(node->parent_node);
			brother = node->parent_node->left_node;
			balance_remove(brother);
		}
		else if (brother != nullptr && brother->color == black)
		{
			if (brother->right_node->color == black && brother->left_node->color == black)
			{
				brother->color = red;
				if (node->parent_node->color == red)
				{
					node->parent_node->color = black;
					return;
				}
				else
					balance_remove(node->parent_node);
			}

			else if (brother->right_node->color == red && brother->left_node->color == black)
			{
				brother->color = red;
				brother->right_node->color = black;
				left_rotate(brother);
				brother = node->parent_node->right_node;
				balance_remove(brother);
			}
			else if (brother->right_node->color == black && brother->left_node->color == red)
			{
				brother->color = node->parent_node->color;
				node->parent_node->color = black;
				brother->left_node->color = black;
				right_rotate(node->parent_node);
				node = root;
			}

		}
		
	}
	
	node->color = black;
}

template<typename T>
TreeNode<T>* MyRBT<T>::remove_node(TreeNode<T>* node, T key)
{
	assert(search_node(node, key) && "Error: Attempting to remove an unexisted element!");
	bool balanceornot = false;
	if (key == node->key)
	{

		if (node->color == red)
		{
			if (!node->left_node && !node->right_node) //means no child just delete
			{
				delete node;
				return nullptr;
			}
			else if (node->left_node && !node->right_node)
			{
				auto temp = node;
				node = node->left_node;
				node->parent_node = temp->parent_node;
				delete temp;
			}
			else if (!node->left_node && node->right_node)
			{
				auto temp = node;
				node = node->right_node;
				node->parent_node = temp->parent_node;
				delete temp;
			}
			else
			{
				auto temp = get_min(node->right_node);
				node->key = temp->key;
				node->right_node = remove_node(node->right_node, temp->key);
			}

			return node;
		}
		else
		{
			if (!node->left_node && !node->right_node) //means no child just delete
			{
				delete node;
				return nullptr;
			}
			else if (node->left_node && !node->right_node)
			{
				auto temp = node;
				node = node->left_node;
				node->parent_node = temp->parent_node;
				if (temp->color == black)
					balanceornot = true;
				if (balanceornot)
					balance_remove(node);
				delete temp;
			}
			else if (!node->left_node && node->right_node)
			{
				auto temp = node;
				node = node->right_node;
				node->parent_node = temp->parent_node;
				if (temp->color == black)
					balanceornot = true;
				if (balanceornot)
					balance_remove(node);
				delete temp;
			}
			else
			{
				auto temp = get_min(node->right_node);
				node->key = temp->key;
				node->right_node = remove_node(node->right_node, temp->key);
			}


			return node;
		}
			
		
	}


	if (key < node->key)
		node->left_node = remove_node(node->left_node, key);
	else if (key > node->key)
		node->right_node = remove_node(node->right_node, key);
	
	
	


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
		std::cout << node->key << "(" << ((node->color) ? "B)-->" : "R)-->") << node->parent_node->key << " " << std::flush;
		//std::cout << "-->Parent-->" << node->parent_node->key << "(" << ((node->parent_node->color) ? "B) " : "R) ") << std::flush;
	}
	inorder(node->right_node);


}


template<typename T>
void MyRBT<T>::print()
{
	assert(root && "Error: Attempting to print an empty tree!");
	std::cout << "The tree is:\n" << std::flush;
	inorder(root);
	std::cout << std::endl;
}

void test()
{
	MyRBT<int> tree;

	int values[] = { 10, 5, 15, 2, 7, 12, 18, 1, 3, 6, 8, -1, 25, 86, -33};
	for (int val : values) {
		std::cout << "\nInserting " << val << "...\n";
		tree.insert(val);
		tree.print();
	}

	tree.remove(15);
	tree.print();
}


int main()
{
	test();


	return 0;
}
