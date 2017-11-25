#pragma once
#ifndef _AVL_TREE_
#define _AVL_TREE_
#include"allocator.h"
#include"type_traits_qmj.h"


namespace qmj
{

	template<typename value_type>
	struct avl_tree_node;

	template<typename value_type>
	struct avl_tree_base_node
	{
		typedef avl_tree_node<value_type>* link_type;

		avl_tree_base_node(link_type p = nullptr,
			link_type left = nullptr,
			link_type right = nullptr,
			int height = 0) :
			p(p), left(left), right(right), height(height)
		{
		}


		link_type p;
		link_type left;
		link_type right;
		int height;
	};

	template<typename value_type>
	struct avl_tree_node :
		avl_tree_base_node<value_type>
	{
		typedef avl_tree_node<value_type>* link_type;
		typedef avl_tree_base_node<value_type> base_type;

		avl_tree_node(const value_type&value,
			link_type p = nullptr,
			link_type left = nullptr,
			link_type right = nullptr,
			int height = 0) :
			value(value), base_type(p, left, right, height)
		{
		}

		avl_tree_node(value_type&&value,
			link_type p = nullptr,
			link_type left = nullptr,
			link_type right = nullptr,
			int height = 0) :
			value(std::move(value)), base_type(p, left, right, height)
		{
		}

		value_type value;
	};

	template<typename value_type>
	class avl_tree
	{
	public:
		typedef avl_tree_node<value_type> node_type;
		typedef node_type* link_type;
		typedef qmj::allocator<node_type> alloc;

		~avl_tree()
		{
			if (root)
				destroy_tree(root);
		}

		link_type insert(const value_type& val)
		{
			return (insert_equal(val));
		}

		link_type find(const value_type&val)
		{
			for (link_type cur = root; cur;)
			{
				if (val < cur->value)
					cur = cur->left;
				else if (val > cur->value)
					cur = cur->right;
				else
					return (cur);
			}
			return (nullptr);
		}

		void erase(link_type tar)
		{
			delete_node(tar);
		}

		void print()
		{
			print(root, 0);
		}

		void print_height()
		{
			cout << "height=" << root->height << endl;
		}

		void print_sorted()
		{
			link_type cur = minimum(root);
			for (; cur;)
			{
				cout << cur->value << " ";
				if (cur->right)
					cur = minimum(cur->right);
				else
				{
					while (cur!=root&&cur->p->right == cur)
						cur = cur->p;
					cur = cur->p;
				}
			}
		}

	private:
		void print(link_type rt, int i = 0)
		{
			if (rt)
			{
				print(rt->right, i + 1);
				for (int n = i; n != -1; --n)
					cout << "\t";
				cout << rt->value << "|" << rt->height << endl;
				cout << endl;
				print(rt->left, i + 1);
			}
		}

	private:

		static link_type minimum(link_type node)
		{
			for (link_type left = node->left; left; left = node->left)
				node = left;
			return (node);
		}

		static link_type maximum(link_type node)
		{
			for (link_type right = node->right; right; right = node->right)
				node = right;
			return (node);
		}

		template<typename value_type>
		link_type insert_equal(value_type&&val)
		{
			link_type cur = root;
			link_type par = cur;
			while (cur)
			{
				par = cur;
				cur = val < cur->value ? cur->left : cur->right;
			}
			link_type tar = create_node(val, par);
			insert_imple(par, tar, val);
			return (tar);
		}

		void insert_imple(link_type par, link_type tar, const value_type&val)
		{
			if (!par)
			{
				root = tar;
				return;
			}

			if (val < par->value)
				par->left = tar;
			else
				par->right = tar;

			int new_height = 1 + max_height(par->left, par->right);
			if (new_height <= par->height)
				return;
			par->height = new_height;
			if (par != root)
				insert_fixup(par->p, val);
		}

		void insert_fixup(link_type cur, const value_type&val)
		{
			int new_height;
			for (link_type par = cur; cur; cur = par)
			{
				par = cur->p;
				link_type left = cur->left;
				link_type right = cur->right;
				int left_height = height(left);
				int right_height = height(right);
				if (val < cur->value)
				{
					if (left_height > right_height)
					{
						if (left_height - right_height == 2)
						{
							if (val > left->value)
							{
								left_rotate_inset(left);
								left->p->height += 1;
							}
							right_rotate_inset(cur);
							return;
						}
						new_height = left_height + 1;
						if (new_height <= cur->height)
							return;
						cur->height = new_height;
					}
					else
						return;
				}
				else
				{
					if (right_height > left_height)
					{
						if (right_height - left_height == 2)
						{
							if (val < right->value)
							{
								right_rotate_inset(right);
								right->p->height += 1;
							}
							left_rotate_inset(cur);
							return;
						}
						new_height = right_height + 1;
						if (new_height <= cur->height)
							return;
						cur->height = new_height;
					}
					else
						return;
				}
			}
		}

		void transplant(link_type node, link_type instead)
		{
			if (node == root)
				root = instead;
			else if (node->p->left == node)
				node->p->left = instead;
			else
				node->p->right = instead;
			if (instead)
				instead->p = node->p;
		}

		void delete_node(link_type tar)
		{
			link_type node = tar->p;//关键节点,删除节点后其高度可能需要调整
			if (!tar->left)
				transplant(tar, tar->right);
			else if (!tar->right)
				transplant(tar, tar->left);
			else
			{
				link_type instead = minimum(tar->right);
				node = instead;
				if (instead->p != tar)
				{
					node = instead->p;
					instead->height = tar->height;
					transplant(instead, instead->right);
					instead->right = tar->right;
					instead->right->p = instead;
				}
				transplant(tar, instead);
				instead->left = tar->left;
				instead->left->p = instead;
			}
			if (!node)//删除的tar是根节点
				node = root;
			if (node)
			{//tar不是树中的唯一节点
				node->height = 1 + max_height(node->left, node->right);
				delete_fixup(node);
			}
			destroy_and_free_node(tar);
		}

		void delete_fixup(link_type tar)
		{
			int new_height = 0;
			for (link_type par = tar; par;)
			{
				tar = par;
				par = tar->p;
				link_type left = tar->left;
				link_type right = tar->right;
				int left_height = height(left);
				int right_height = height(right);
				if (right_height < left_height)
				{
					if (left_height - right_height == 2)
					{
						if (height(left->right) > height(left->left))
							left_rotate_del(left);
						right_rotate_del(tar);
						return;
					}
					new_height = left_height + 1;
					if (new_height <= tar->height)
						return;
					tar->height = new_height;
				}
				else
				{
					if (right_height - left_height == 2)
					{
						if (height(right->left) > height(right->right))
							right_rotate_del(right);
						left_rotate_del(tar);
						return;
					}
					new_height = right_height + 1;
					if (new_height <= tar->height)
						return;
					tar->height = new_height;
				}
			}
		}

	private:
		template<typename...types>
		link_type create_node(types&&...args)
		{
			link_type tar = alloc::allocate();
			alloc::construct(tar, std::forward<types>(args)...);
			return (tar);
		}

		void destroy_and_free_node(link_type node)
		{
			alloc::destroy(node);
			alloc::deallocate(node);
		}

		void destroy_tree(link_type cur)
		{
			if (cur->left)
				destroy_tree(cur->left);
			if (cur->right)
				destroy_tree(cur->right);
			destroy_and_free_node(cur);
		}

		int height(link_type tar)
		{
			return (tar ? tar->height : -1);
		}

		int max_height(link_type left, link_type right)
		{
			int left_height = height(left);
			int right_height = height(right);
			if (left_height < right_height)
				left_height = right_height;
			return (left_height);
		}

		void left_rotate_imple(link_type tar, link_type tarRight)
		{
			if (tar == root)
				root = tarRight;
			else if (tar == tar->p->left)
				tar->p->left = tarRight;
			else
				tar->p->right = tarRight;
			tarRight->p = tar->p;

			if (tarRight->left)
				tarRight->left->p = tar;
			tar->right = tarRight->left;

			tarRight->left = tar;
			tar->p = tarRight;
		}

		void right_rotate_imple(link_type tar, link_type tarLeft)
		{
			if (tar == root)
				root = tarLeft;
			else if (tar == tar->p->left)
				tar->p->left = tarLeft;
			else
				tar->p->right = tarLeft;
			tarLeft->p = tar->p;

			if (tarLeft->right)
				tarLeft->right->p = tar;
			tar->left = tarLeft->right;

			tarLeft->right = tar;
			tar->p = tarLeft;
		}


		void left_rotate_del(link_type tar)
		{
			link_type tarRight = tar->right;
			left_rotate_imple(tar, tarRight);
			tar->height = 1 + max_height(tar->left, tar->right);
			tarRight->height = 1 + max_height(tarRight->left, tarRight->right);
		}

		void right_rotate_del(link_type tar)
		{
			link_type tarLeft = tar->left;
			right_rotate_imple(tar, tarLeft);
			tar->height = 1 + max_height(tar->left, tar->right);
			tarLeft->height = 1 + max_height(tarLeft->left, tarLeft->right);
		}

		void left_rotate_inset(link_type tar)
		{
			link_type tarRight = tar->right;
			left_rotate_imple(tar, tarRight);
			tar->height -= 1;
		}

		void right_rotate_inset(link_type tar)
		{
			link_type tarLeft = tar->left;
			right_rotate_imple(tar, tarLeft);
			tar->height -= 1;
		}

	private:

		link_type root = nullptr;
	};

}



#endif
