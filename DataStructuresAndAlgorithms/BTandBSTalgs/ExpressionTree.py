"""
Author: Jonny Sassoon
Program: Expression Tree
This function (along with its helper function) creates and returns an expression tree
from a prefix expression string
"""

from DataStructures.BinaryTree import LinkedBinaryTree

def create_subtree(data, left_child = None, right_child = None): # helper function from BinaryTree program
    subtree_root = LinkedBinaryTree.Node(data, None, left_child, right_child)
    if left_child:
        left_child.parent = subtree_root
    if right_child:
        right_child.parent = subtree_root
    return subtree_root

def create_expression_tree(prefix_exp_string):
	prefix_exp = prefix_exp_string.split()
	data_tuple = create_expression_tree_helper(prefix_exp, 0)
	root = data_tuple[0]
	tree = LinkedBinaryTree(root)
	return tree

def create_expression_tree_helper(prefix_exp, start_pos):
	if prefix_exp[start_pos].isdigit():
		sub_tree = create_subtree(int(prefix_exp[start_pos]))
		return (sub_tree, 1)
	else:
		curr_root = prefix_exp[start_pos]
		left_tree = create_expression_tree_helper(prefix_exp, start_pos+1)
		step = left_tree[1]
		start_pos += 1
		right_tree = create_expression_tree_helper(prefix_exp, start_pos+step)
		step += right_tree[1]
		sub_tree = create_subtree(curr_root, left_tree[0], right_tree[0])
		return (sub_tree, step+1)

def add(x, y): return x+y
def sub(x, y): return x-y
def mul(x, y): return x*y
def div(x, y): return x/y

operators_dict = {'+':add, '-':sub, '*':mul, '/':div}

def eval(expression_tree):
	"""
	:param expression_tree: Binary expression tree
	:return: result of expression
	"""
	return eval_subtree(expression_tree.root)

def eval_subtree(curr_root):
	if curr_root.left == None and curr_root.right == None:
		return curr_root.data
	else:
		operand_1 = eval_subtree(curr_root.left)
		operand_2 = eval_subtree(curr_root.right)
		operator = operators_dict[curr_root.data]
		return operator(operand_1, operand_2)