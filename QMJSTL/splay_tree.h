#pragma once
#ifndef _SPLAY_TREE_
#define _SPLAY_TREE_
#include"allocator.h"
#include"stack_qmj.h"

namespace qmj
{
	template<typename value_type>
	struct splay_tree_node;

	template<typename value_type>
	struct splay_tree_base_node
	{
		typedef splay_tree_node<value_type>* link_type;
		splay_tree_base_node(link_type left = nullptr,
			link_type right = nullptr) :
			left(left), right(right)
		{
		}

		link_type left;
		link_type right;
	};

	template<typename value_type>
	struct splay_tree_node :
		public splay_tree_base_node<value_type>
	{
		typedef splay_tree_base_node<value_type> base_type;
		typedef splay_tree_node<value_type>* link_type;
		splay_tree_node(const value_type&val,
			link_type left = nullptr,
			link_type right = nullptr) :
			value(val), base_type(left, right)
		{
		}

		splay_tree_node(value_type&&val,
			link_type left = nullptr,
			link_type right = nullptr) :
			value(std::move(val)), base_type(left, right)
		{
		}

		splay_tree_node(link_type left = nullptr,
			link_type right = nullptr) :
			base_type(left, right)
		{
		}

		value_type value;
	};

	template<typename value_type>
	class splay_tree
	{
	public:
		typedef splay_tree_node<value_type> node_type;
		typedef typename node_type::base_type node_base_type;
		typedef node_type* link_type;

		typedef qmj::allocator<node_type> alloc;
		typedef std::pair<link_type, bool> PairLB;

		splay_tree()
		{
			create_nil();
			root = nil;
		}

		~splay_tree()
		{
			destroy_tree();
			destroy_and_free_node(nil);
		}

		link_type insert(const value_type&val)
		{
			return (insert_unique(val));
		}

		link_type find(const value_type&val)
		{
			return (find_imple(val));
		}

		void erase(link_type node)
		{
			delete_node(node);
		}

		void print()
		{
			print(root, 0);
		}

		void inorder_walk()
		{
			qmj::stack<link_type>stk;
			link_type cur = root;
			link_type next;
			do
			{
				while (cur != nil)
				{
					stk.push(cur);
					cur = cur->left;
				}
				next = stk.top();
				stk.pop();
				cur = next->right;
				cout << next->value << " ";
			} while (!stk.empty() || cur != nil);
		}

		void inorder_walk(int)
		{//no stack,Morris±éÀú
			link_type par = root;
			link_type left;
			while (par != nil)
			{
				left = par->left;
				if (left == nil)
				{
					cout << par->value << " ";
					par = par->right;
				}
				else
				{
					link_type tmp = left;
					while (tmp->right != nil&&tmp->right != par)
						tmp = tmp->right;
					if (tmp->right == nil)
					{
						tmp->right = par;
						par = left;
					}
					else
					{
						cout << par->value << " ";
						tmp->right = nil;
						par = par->right;
					}
				}
			}
		}

	private:
		void print(link_type cur, int dep = 0)
		{
			if (cur != nil)
			{
				print(cur->right, dep + 1);
				for (int n = dep; n != -1; --n)
					cout << "\t";
				cout << cur->value << endl;
				print(cur->left, dep + 1);
			}
		}

	private:

		link_type minimum(link_type node)
		{
			for (link_type cur = node->left; cur != nil; cur = node->left)
				node = cur;
			return (node);
		}

		link_type maximum(link_type node)
		{
			for (link_type cur = node->right; cur != nil; cur = node->right)
				node = cur;
			return (node);
		}

		link_type left_rotate(link_type tar)
		{
			link_type tarLeft = tar->left;
			tar->left = tarLeft->right;
			tarLeft->right = tar;
			return (tarLeft);
		}

		link_type right_rotate(link_type tar)
		{
			link_type tarRight = tar->right;
			tar->right = tarRight->left;
			tarRight->left = tar;
			return (tarRight);
		}

		PairLB splay(const value_type& val, link_type cur)
		{
			link_type left_max = nil;
			link_type right_min = nil;
			nil->value = val;
			bool exist = false;
			nil->left = nil->right = nil;
			for (;;)
			{
				if (val < cur->value)
				{
					if (val < cur->left->value)
						cur = left_rotate(cur);
					if (cur->left == nil)
						break;
					right_min->left = cur;
					right_min = cur;
					cur = cur->left;
				}
				else if (cur->value < val)
				{
					if (cur->right->value < val)
						cur = right_rotate(cur);
					if (cur->right == nil)
						break;
					left_max->right = cur;
					left_max = cur;
					cur = cur->right;
				}
				else
				{
					exist = true;
					break;
				}
			}

			left_max->right = cur->left;
			right_min->left = cur->right;
			cur->left = nil->right;
			cur->right = nil->left;
			return (PairLB(cur, exist));
		}

		link_type insert_unique(const value_type&val)
		{
			link_type node = create_node(val);
			if (root == nil)
				root = node;
			else
			{
				root = splay(val, root).first;
				if (val < root->value)
				{
					node->left = root->left;
					node->right = root;
					root->left = nil;
					root = node;
				}
				else if (root->value < val)
				{
					node->right = root->right;
					node->left = root;
					root->right = nil;
					root = node;
				}
			}
			return (root);
		}

		link_type find_imple(const value_type&val)
		{
			PairLB ret = splay(val, root);
			root = ret.first;
			return (ret.second ? root : nil);
		}

		void delete_node(link_type node)
		{
			link_type new_root;
			root = splay(node->value, root).first;
			if (root->left == nil)
				new_root = root->right;
			else
			{
				new_root = root->left;
				new_root = splay(node->value, new_root).first;//new_node=maximum(root->left)
				new_root->right = root->right;
			}
			destroy_and_free_node(node);
			root = new_root;
		}

	private:
		void create_nil()
		{
			nil = alloc::allocate();
			alloc::construct(nil, nil, nil);
		}

		template<typename value_type>
		link_type create_node(value_type&&val)
		{
			link_type node = alloc::allocate();
			alloc::construct(node, std::forward<value_type>(val), nil, nil);
			return (node);
		}

		void destroy_and_free_node(link_type node)
		{
			qmj::allocator<node_base_type>::destroy(&(node->value));
			alloc::deallocate(node);
		}

		void destroy_tree()
		{
			link_type stk = nil;
			link_type cur = root;
			link_type next, left;
			nil->left = nil->right = nil;
			for (;;)
			{
				while (cur != nil)
				{
					left = cur->left;
					cur->left = stk;
					stk = cur;
					cur = left;
				}
				next = stk;
				stk = stk->left;
				cur = next->right;
				if (stk == nil&&cur == nil)
					return;
				destroy_and_free_node(next);
			}
		}

	private:
		link_type root;
		link_type nil;
	};

}

#endif
