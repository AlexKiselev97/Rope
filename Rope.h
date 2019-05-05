#include <string>

#ifndef ROPE_H_
#define ROPE_H_

class Rope
{
	struct Node {
		int size_ = 1;
		char c_ = '#';
		Node* left_ = nullptr;
		Node* right_ = nullptr;
		Node* parent_ = nullptr;
		Node(char key, Node* node) : c_(key), parent_(node) {}
		void updateSize();
	};

	Node* root_ = nullptr;

	void insert_(char c, int pos, Node* root);
	void remove_(Node* x);
	void print_(Node* root);
	void push_back_(char c, Node* root);
	std::string getString_(Node* root);

	void zig(Node* x, Node* p);
	void zag(Node* x, Node* p);
	void splay(Node* x);
	Node* merge(Node* l, Node* r);
	Node* maxInTree(Node* root);
	std::pair<Node*, Node*> split(Node* root, int key);
	Node* find(int key, Node* root);
public:
	void insert(char c, int pos);
	void remove(int pos);
	void print() { print_(root_); }
	void push_back(char c);
	std::string getString() { return getString_(root_); }
	void reorder(int l, int r, int k);
	char& operator[](int pos);
};

#endif ROPE_H_