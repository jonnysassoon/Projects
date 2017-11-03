"""
Author: Jonny Sassoon
Program: Postorder generator
Creates an implicit postorder list from inputted inorder and preorder traversal lists
"""
def gen_postorder(preorder, inorder):
    """
    :param preorder: List of Preorder traversal
    :param inorder: List of Inorder traversal
    :return: List of corresponding Postorder traversal
    """
    if len(inorder) <= 1:
        return inorder
    else:
        ind = 0
        while inorder[ind] != preorder[0]:
            ind += 1
        root = ind
        root_data = preorder[0]
        left = gen_postorder(preorder[1:], inorder[:root])
        right = gen_postorder(preorder[root+1:], inorder[root+1:])
        return left + right + [root_data]