#include <iostream>
#include <memory>
#include <cassert>

//the color of the node
#define red false
#define black true


void test();
//v9 Black-Red tree
//We need to make sure that all the insertion node should be red
//but if the tree is empty, after inserting we need to covert the node to black, since all the root should be black
//no constant red node, but black is not limited
//from each node to its leaf(null) node, the numbers of the black node should be same



template<typename Key, typename Value>
struct TreeNode
{
	Key key; //the key(index)
	Value value; ///the value(element)
	bool color; ///color
	TreeNode<Key, Value>* parent_node;
	TreeNode<Key, Value>* left_node;
	TreeNode<Key, Value>* right_node;

	TreeNode(Key key, Value value) : //define the new node always be the red
		color(red), key(key), value(value), parent_node(nullptr), left_node(nullptr), right_node(nullptr)
	{
		//std::cout << "Constructing node..." << std::endl;
	}
	~TreeNode()
	{
		//std::cout << "Destructing node..." << std::endl;
	}
};

template<typename Key, typename Value>
class MyRBT
{
public:

	MyRBT() :root(nullptr)
	{
		leaf_node = new TreeNode<Key, Value>(Key(), Value());
		leaf_node->color = black;
		leaf_node->left_node = leaf_node->right_node = leaf_node->parent_node = nullptr;
		//std::cout << "Constructing tree..." << std::endl;
	}
	~MyRBT()
	{
		clear(root);
		//std::cout << "Destructing tree..." << std::endl;
	}
	/****************************************Iterator*****************************************************************/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Iterator
	{

	public:
		Iterator(TreeNode<Key, Value>* node, MyRBT<Key, Value>* tree) : t_node(node), tree(tree) {}
		std::pair<Key, Value> operator*()
		{
			assert(t_node && "Error: Attempt to access an empty node!");
			return { t_node->key, t_node->value };
		}

		Iterator& operator++()
		{
			assert(t_node && "Error: Out of range!");
			if (t_node->right_node) //from small to big
			{
				t_node = tree->get_min(t_node->right_node);
			}
			else
			{
				TreeNode<Key, Value>* temp_parent = t_node->parent_node;
				while (temp_parent && t_node == temp_parent->right_node)
				{
					t_node = temp_parent;
					temp_parent = temp_parent->parent_node;
				}
				t_node = temp_parent;
			}

			return *this;
		}

		Iterator& operator++(int)
		{
			assert(t_node && "Error: Out of range!");
			auto temp = *this;
			if (t_node->right_node)
			{
				t_node = tree->get_min(t_node->right_node);
			}
			else
			{
				TreeNode<Key, Value>* temp_parent = t_node->parent_node;
				while (temp_parent && t_node == temp_parent->right_node)
				{
					t_node = temp_parent;
					temp_parent = temp_parent->parent_node;
				}
				t_node = temp_parent;
			}

			return temp;
		}

		Iterator& operator--()
		{
			assert(t_node->key != (*(tree->begin())).first && "Error: Out of range!");
			if (t_node == nullptr) //if end()
			{
				t_node = tree->get_max(tree->root);
				return *this;
			}

			if (t_node->left_node) //from big->samll
			{
				t_node = tree->get_max(t_node->left_node);
			}
			else
			{
				TreeNode<Key, Value>* temp_parent = t_node->parent_node;
				while (temp_parent && t_node == temp_parent->left_node)
				{
					t_node = temp_parent;
					temp_parent = temp_parent->parent_node;
				}
				t_node = temp_parent;
			}

			return *this;
		}

		Iterator& operator--(int)
		{
			assert(t_node->key != (*(tree->begin())).first && "Error: Out of range!");
			auto temp = *this;
			if (t_node == nullptr && tree)
			{
				t_node = tree->get_max(tree->root);
				return temp;
			}

			if (t_node->left_node)
			{
				t_node = MyRBT<Key, Value>::get_max(t_node->left_node);
			}
			else
			{
				TreeNode<Key, Value>* temp_parent = t_node->parent_node;
				while (temp_parent && t_node == temp_parent->left_node)
				{
					t_node = temp_parent;
					temp_parent = temp_parent->parent_node;
				}
				t_node = temp_parent;
			}

			return temp;
		}

		bool operator==(const Iterator& rhs) const
		{
			return t_node == rhs.t_node;
		}

		bool operator!=(const Iterator& rhs) const
		{
			return !(*this == rhs);
		}
	private:
		TreeNode<Key, Value>* t_node;
		MyRBT<Key, Value>* tree;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   /****************************************Iterator*****************************************************************/


	Value& operator[](const Key& key)
	{
		//assert(root && "Error: Attempting to access an empty map!");

		TreeNode<Key, Value>* cur_node = root;
		while (cur_node != nullptr) //check the node exist or not
		{
			if (key == cur_node->key)
				return cur_node->value; //if exist
			if (key < cur_node->key)
				cur_node = cur_node->left_node;
			else if (key > cur_node->key)
				cur_node = cur_node->right_node;
		}
		root = insert_node(root, key, Value()); //not exist insert
		return operator[](key); 

	}

	Iterator begin() { return Iterator(get_min(this->root), this); }; //the smallest element
	Iterator end() { return Iterator(nullptr, this); }; //nullptr or we call it leafnode

	Value& at(const Key& key) //find the node
	{
		assert(search_node(root, key) && "Error: Attempt to search an unexisted element!");
		return operator[](key);
	}

	void insert(Key key, Value value)
	{
		root = insert_node(root, key, value);
	}

	void remove(Key key)
	{
		root = remove_node(root, key);
	}

	bool search(Key key)
	{
		return search_node(root, key);
	}

	void print();

private:

	TreeNode<Key, Value>* root;
	TreeNode<Key, Value>* leaf_node;

	TreeNode<Key, Value>* insert_node(TreeNode<Key, Value>*, Key, Value);
	TreeNode<Key, Value>* remove_node(TreeNode<Key, Value>*, Key);
	TreeNode<Key, Value>* get_max(TreeNode<Key, Value>*);
	TreeNode<Key, Value>* get_min(TreeNode<Key, Value>*);
	bool search_node(TreeNode<Key, Value>*, Key);
	void inorder(TreeNode<Key, Value>*); //for print
	void clear(TreeNode<Key, Value>* node); //for print

	void left_rotate(TreeNode<Key, Value>*);
	void right_rotate(TreeNode<Key, Value>*);
	void balance_insert(TreeNode<Key, Value>*);
	void balance_remove(TreeNode<Key, Value>*);

};

template<typename Key, typename Value>
TreeNode<Key, Value>* MyRBT<Key, Value>::insert_node(TreeNode<Key, Value>* cur_node, Key key, Value value)
{
	TreeNode<Key, Value>* new_node = new TreeNode<Key, Value>(key, value); //the new node that we'll insert
	TreeNode<Key, Value>* last_node = nullptr; //used to be the parent

	while (cur_node != nullptr) //if there are nodes in the tree
	{
		last_node = cur_node;
		assert(key != last_node->key && "Error: Attempting to insert an exist element!");
		if (key < cur_node->key)
			cur_node = cur_node->left_node;
		else
			cur_node = cur_node->right_node;
	}

	new_node->parent_node = last_node;

	if (last_node == nullptr) //no nodes in the tree
		root = new_node;
	else if (key < last_node->key)
		last_node->left_node = new_node;
	else
		last_node->right_node = new_node;


	new_node->left_node = nullptr; //leafnode
	new_node->right_node = nullptr;

	balance_insert(new_node); //balance from the insert node
	return root;
}
template<typename Key, typename Value>
void MyRBT<Key, Value>::balance_insert(TreeNode<Key, Value>* node)
{

	if (node == root) //finish
	{
		//std::cout << "Finsih balancing..." << std::endl;
		node->color = black;
		return;
	}
	if (node->parent_node->color == black) //finish
	{
		return;
	}

	TreeNode<Key, Value>* grandparent = node->parent_node->parent_node;
	TreeNode<Key, Value>* uncle = nullptr;

	//balance case
	//left side
	if (node->parent_node->key < grandparent->key)
	{
		//std::cout << "Start balancing left side..." << std::endl;
		uncle = grandparent->right_node;
		if (uncle != nullptr && uncle->color == red)
		{
			//std::cout << "Uncle is: " << uncle->key << std::endl;
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
	//right side
	else
	{
		//std::cout << "Start balancing right side..." << std::endl;
		uncle = grandparent->left_node;
		if (uncle != nullptr && uncle->color == red)
		{
			//std::cout << "Uncle is: " << uncle->key << std::endl;
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

template<typename Key, typename Value>
void MyRBT<Key, Value>::left_rotate(TreeNode<Key, Value>* node)
{
	if (node == nullptr || node->right_node == nullptr)
		return;
	TreeNode<Key, Value>* temp = node->right_node;
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

template<typename Key, typename Value>
void MyRBT<Key, Value>::right_rotate(TreeNode<Key, Value>* node)
{
	if (node == nullptr || node->left_node == nullptr)
		return;
	TreeNode<Key, Value>* temp = node->left_node;
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




template<typename Key, typename Value>
TreeNode<Key, Value>* MyRBT<Key, Value>::get_max(TreeNode<Key, Value>* node)
{

	if (node && node->right_node)
		node = get_max(node->right_node);

	return node;
}



template<typename Key, typename Value>
TreeNode<Key, Value>* MyRBT<Key, Value>::get_min(TreeNode<Key, Value>* node)
{

	if (node && node->left_node)
		node = get_min(node->left_node);

	return node;
}

template<typename Key, typename Value>
void MyRBT<Key, Value>::balance_remove(TreeNode<Key, Value>* node)
{


	if (node == root)
		return;

	TreeNode<Key, Value>* brother;

	if (node->key < node->parent_node->key)
	{
		brother = node->parent_node->right_node;
		if (brother->right_node == nullptr)
			brother->right_node = leaf_node;


		if (brother->left_node == nullptr)
			brother->left_node = leaf_node;

		if (brother != nullptr && brother->color == red)
		{
			brother->color = black;
			node->parent_node->color = red;
			left_rotate(node->parent_node);
			brother = node->parent_node->right_node;
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

template<typename Key, typename Value>
TreeNode<Key, Value>* MyRBT<Key, Value>::remove_node(TreeNode<Key, Value>* node, Key key)
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


template<typename Key, typename Value>
bool MyRBT<Key, Value>::search_node(TreeNode<Key, Value>* node, Key key)
{
	assert(root && "Error: Attempting to search an empty tree!");
	if (!node)
		return false;
	if (key == node->key)
		return true;

	return (key < node->key) ? search_node(node->left_node, key) : search_node(node->right_node, key);
}



template<typename Key, typename Value>
void MyRBT<Key, Value>::inorder(TreeNode<Key, Value>* node)
{
	if (!node)
		return;
	inorder(node->left_node);
	if (node == root)
		std::cout << "\n***The root is: " << node->key << "(" << ((node->color) ? "B)" : "R)") << "-->Value:" << node->value << "***" << std::endl;
	else
	{
		std::cout << "Key:" << node->key << "(" << ((node->color) ? "B)" : "R)") << "-->Value:" << node->value << "-->Parent:"
			<< node->parent_node->key << " " << std::flush;
		//std::cout << "-->Parent-->" << node->parent_node->key << "(" << ((node->parent_node->color) ? "B) " : "R) ") << std::flush;
	}
	inorder(node->right_node);


}

template<typename Key, typename Value>
void MyRBT<Key, Value>::clear(TreeNode<Key, Value>* node)
{
	
	if (node != nullptr)
	{
		clear(node->left_node);
		clear(node->right_node);
		delete node;
	}

}


template<typename Key, typename Value>
void MyRBT<Key, Value>::print()
{
	assert(root && "Error: Attempting to print an empty tree!");
	std::cout << "The tree is:\n" << std::flush;
	inorder(root);
	std::cout << std::endl;
}

void test()
{
	/*MyRBT<int, std::string> tree;

	int values[] = { 10, 5, 15, 2, 7, 12, 18, 1, 3, 6, 8, -1, 25, 86, -33 };
	for (int val : values) {
		std::cout << "\nInserting " << val << "...\n";
		tree.insert(val, "Hi");
		tree.print();
	}

	tree.remove(15);
	tree.print();
	tree[12] = "Hello";
	tree.print();*/


	MyRBT<std::string, int> m;

	m["delta"] = 4;
	m["alpha"] = 1;
	m["epsilon"] = 5;
	m["beta"] = 2;
	m["gamma"] = 3;
	m.print();

	auto beg = m.begin();
	while (beg != m.end())
	{
		std::cout << (*beg).first << std::endl;
		++beg;
	}

	
}


int main()
{
	test();


	return 0;
}
