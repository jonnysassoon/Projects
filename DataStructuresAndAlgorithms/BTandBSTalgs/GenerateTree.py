"""
Author: Jonny Sassoon
Program: Generate Tree
This function generates a tree corresponding to inputted preoder and inorder traversal lists
"""
from DataStructures.BinaryTree import LinkedBinaryTree

def gen_tree(preorder, inorder):
    """
    :param preorder: List of resulting Preorder traversal
    :param inorder: List of Resulting Inorder traversal
    :return: LinkedBinaryTree.node of corresponding tree from these lists
    """
    if len(inorder) == 1:
        return LinkedBinaryTree.node(inorder[0])
    else:
        ind = 0
        while inorder[ind] != preorder[0]:
            ind += 1
        root = ind
        root_data = preorder[0]
        left = gen_tree(preorder[1:], inorder[:root])
        right = gen_tree(preorder[root+1:], inorder[root+1:])
        tree = LinkedBinaryTree.node(root_data, None, left, right)
        return tree