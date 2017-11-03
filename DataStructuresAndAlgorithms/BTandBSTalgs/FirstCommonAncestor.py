"""
Author: Jonny Sassoon
Program: First Common Ancestor
Two functions that return the first common ancestor of two nodes
in a Binary Search Tree and a Binary Tree, respectively
"""

from DataStructures.BinaryTree import LinkedBinaryTree

def fca_bt(bt, node_a, node_b):
    """
    Returns first common ancestor in binary tree
    :param bt: LinkedBinaryTree
    :param node_a: LinkedBinaryTree.node
    :param node_b: LinkedBinaryTree.node
    :return: LinkedBinaryTree.node
    """
    node_a_to_root = [node_a]
    node_b_to_root = [node_b]
    while node_a.parent:
        node_a_to_root.append(node_a.parent)
        node_a = node_a.parent
    while node_b.parent:
        node_b_to_root.append(node_b.parent)
        node_b = node_b.parent
    while node_a_to_root[-1] == node_b_to_root[-1]:
        node_b_to_root.pop()
        previous = node_a_to_root.pop()

    return previous