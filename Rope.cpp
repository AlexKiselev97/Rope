#include "Rope.h"
#include <iostream>

void Rope::push_back(char c)
{
	push_back_(c, root_);
}

void Rope::push_back_(char c, Node* root)
{
	if (root == nullptr) // empty tree
	{
		root_ = new Node(c, nullptr);
	}
	else
	{
		if (root->right_ == nullptr)
		{
			root->right_ = new Node(c, root);
			splay(root->right_);
		}
		else
		{
			push_back_(c, root->right_);
		}
	}
}

void Rope::reorder(int l, int r, int k)
{
	++l;
	++r;
	++k;

	std::pair<Node*, Node*> l_and_x = split(root_, l); // (..., L) and [X, ...]
	r = r - l + 2;
	std::pair<Node*, Node*> x_and_r = split(l_and_x.second, r); // [X, R] and (R, ...)
	root_ = merge(l_and_x.first, x_and_r.second); // (..., L) + (R, ...)
	std::pair<Node*, Node*> l_plus_k_r = split(root_, k); // [..., k) and [k, ...]
	root_ = merge(merge(l_plus_k_r.first, x_and_r.first), l_plus_k_r.second); //  [..., k) + X + [k, ...]
}

char& Rope::operator[](int pos)
{
	pos++;
	return find(pos, root_)->c_;
}

void Rope::splay(Node* x)
{
	if (x == nullptr)
		return;

	if (x->parent_ != nullptr)
	{
		Node* p = x->parent_;
		if (p->parent_ == nullptr)
		{ // zig/zag
			if (p->left_ == x)
				zig(x, p);
			else
				zag(x, p);
		}
		else
		{
			Node* g = p->parent_;
			if (g->left_ == p && p->left_ == x)
			{ // zig-zig
				zig(p, g);
				zig(x, p);
			}
			else if (g->right_ == p && p->right_ == x)
			{ // zag-zag
				zag(p, g);
				zag(x, p);
			}
			else
			{
				if (p->left_ == x)
				{ // zig-zag
					zig(x, p);
					g->right_ = x;
					zag(x, g);
				}
				else
				{ // zag-zig
					zag(x, p);
					g->left_ = x;
					zig(x, g);
				}
			}
		}
		splay(x);
	}
	else
	{
		root_ = x;
	}
}

void Rope::zig(Node * x, Node * p)
{
	p->left_ = x->right_;
	if (p->left_ != nullptr)
		p->left_->parent_ = p;
	x->right_ = p;
	x->parent_ = p->parent_;
	p->parent_ = x;
	if (x->parent_ != nullptr)
	{
		if (x->parent_->left_ == p)
			x->parent_->left_ = x;
		else
			x->parent_->right_ = x;
	}
	p->updateSize();
	x->updateSize();
}

void Rope::zag(Node * x, Node * p)
{
	p->right_ = x->left_;
	if (p->right_ != nullptr)
		p->right_->parent_ = p;
	x->left_ = p;
	x->parent_ = p->parent_;
	p->parent_ = x;
	if (x->parent_ != nullptr)
	{
		if (x->parent_->left_ == p)
			x->parent_->left_ = x;
		else
			x->parent_->right_ = x;
	}
	p->updateSize();
	x->updateSize();
}

Rope::Node* Rope::find(int pos, Node * root)
{
	if (root == nullptr)
		return root;

	int node_index = root->size_;
	if (root->right_ != nullptr)
		node_index -= root->right_->size_;
	
	if (pos == node_index)
		return root;
	else
	{
		if (pos < node_index)
			return find(pos, root->left_);
		else
			return find(pos - node_index, root->right_);
	}
}

void Rope::remove(int pos)
{
	++pos;
	remove_(find(pos, root_));
}
 
void Rope::remove_(Node * x)
{
	if (x == nullptr)
		return;

	if (x->left_ == nullptr && x->right_ == nullptr && x->parent_ == nullptr)
	{ // root of tree
		delete x;
		root_ = nullptr;
	}
	else
	{
		splay(x);
		root_ = merge(x->left_, x->right_);
	}
}

Rope::Node* Rope::merge(Node * l, Node * r)
{
	if (l == nullptr && r == nullptr)
	{
		return nullptr;
	}
	else if (l == nullptr)
	{
		r->parent_ = nullptr;
		return r;
	}
	else if (r == nullptr)
	{
		l->parent_ = nullptr;
		return l;
	}
	else
	{
		Node* m = maxInTree(l);
		splay(m);
		m->right_ = r;
		r->parent_ = m;
		m->updateSize();
		return m;
	}
}

Rope::Node* Rope::maxInTree(Node * root)
{
	if (root == nullptr)
		return root;

	if (root->right_ != nullptr)
		return maxInTree(root->right_);
	else
		return root;
}

void Rope::print_(Node * root)
{ // for console debug
	if (root == nullptr)
		return;
	print_(root->left_);
	std::cout << '(' << root->c_ << ":s=" << root->size_ <<
		", p=" << root->parent_ << ", l=" << root->left_ << ", r=" << root->right_ << ", me=" << root << ")\n";
	print_(root->right_);
}

std::pair<Rope::Node*, Rope::Node*> Rope::split(Node * root, int pos)
{
	Node* x = find(pos, root);
	splay(x);
	if (x == nullptr) // whole tree less key
	{
		return { root, x };
	}
	else
	{
		if (x->left_ != nullptr)
			x->left_->parent_ = nullptr;
		Node * less = x->left_;
		x->left_ = nullptr;
		x->updateSize();
		return { less, x };
	}
}

void Rope::insert(char c, int pos)
{
	insert_(c, pos, root_);
}

void Rope::insert_(char c, int pos, Node * root)
{
	if (root == nullptr) // empty tree
	{
		root_ = new Node(c, nullptr);
	}
	else
	{
		int node_index = root->size_;
		if (root->right_ != nullptr)
			node_index -= root->right_->size_;

		if (pos < node_index)
		{
			if (root->left_ == nullptr)
			{
				root->left_ = new Node(c, root);
				splay(root->left_);
			}
			else
				insert_(c, pos, root->left_);
		}
		else
		{
			if (root->right_ == nullptr)
			{
				root->right_ = new Node(c, root);
				splay(root->right_);
			}
			else
				insert_(c, pos - node_index, root->right_);
		}
	}
}

std::string Rope::getString_(Node* root)
{
	if (root == nullptr)
		return "";

	return  getString_(root->left_) + root->c_ + getString_(root->right_);
}

void Rope::Node::updateSize()
{
	size_ = 1;
	if (left_ != nullptr)
		size_ += left_->size_;
	if (right_ != nullptr)
		size_ += right_->size_;
}