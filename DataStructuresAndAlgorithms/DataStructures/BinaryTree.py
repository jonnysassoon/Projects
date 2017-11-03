"""
Author: Jonny Sassoon
Program: Binary Tree
Implementation of Binary Tree class where Nodes have ints as data
"""

from DataStructures.ArrayQueue import ArrayQueue

class Empty(Exception):
    pass

class LinkedBinaryTree:
    class Node:
        def __init__(self, data, parent=None, left=None, right=None):
            self.right = right
            self.left = left
            self.parent = parent
            self.data = data

    def __init__(self, root=None):
        self.root = root
        self.size = self.subtree_count(self.root)

    def __len__(self):
        return self.size

    def is_empty(self):
        return len(self) == 0

    def subtree_count(self, curr_root):
        if curr_root == None:
            return 0
        else:
            left_count = self.subtree_count(curr_root.left)
            right_count = self.subtree_count(curr_root.right)
            return left_count + right_count + 1

    def sum(self):
        return self.subtree_sum(self.root)

    def subtree_sum(self, curr_root):
        if curr_root == None:
            return 0
        else:
            left_sum = self.subtree_sum(curr_root.left)
            right_sum = self.subtree_sum(curr_root.right)
            return left_sum + right_sum + curr_root.data

    def preorder(self):
        yield from self.subtree_preorder(self.root)

    def subtree_preorder(self, curr_root):
        if curr_root == None:
            pass
        else:
            yield curr_root  # creates iterable collection of nodes
            yield from self.subtree_preorder(curr_root.left)
            yield from self.subtree_preorder(curr_root.right)

    def postorder(self):
        yield from self.subtree_postorder(self.root)

    def subtree_postorder(self, curr_root):
        if curr_root == None:
            pass
        else:
            yield from self.subtree_postorder(curr_root.left)
            yield from self.subtree_postorder(curr_root.right)
            yield curr_root

    def inorder(self):
        yield from self.subtree_inorder(self.root)

    def subtree_inorder(self, curr_root):
        if curr_root == None:
            pass
        else:
            yield from self.subtree_inorder(curr_root.left)
            yield curr_root
            yield from self.subtree_inorder(curr_root.right)

    def breadth_first(self):
        if self.is_empty():
            pass
        else:
            nodes_q = ArrayQueue()
            nodes_q.enqueue(self.root)
            while not nodes_q.is_empty():
                curr_node = nodes_q.dequeue()
                yield curr_node
                if curr_node.left != None:
                    nodes_q.enqueue(curr_node.left)
                if curr_node.right != None:
                    nodes_q.enqueue(curr_node.right)

    def __iter__(self):
        for node in self.breadth_first():
            yield node.data

    def height(self):
        return self.subtree_height(self.root)

    def subtree_height(self, curr_root):
        if curr_root.left == None and curr_root.right == None:
            return 0
        elif curr_root.left == None:
            return 1 + self.subtree_height(curr_root.right)
        elif curr_root.right == None:
            return 1 + self.subtree_height(curr_root.left)
        else:
            left_height = self.subtree_height(curr_root.left)
            right_height = self.subtree_height(curr_root.right)
            return max(left_height, right_height) + 1


    def max(self):
        """
        :return: max elem in tree
        """
        if self.is_empty():
            raise Empty("LinkedBinaryTree is empty")
        return self.subtree_max(self.root)

    def subtree_max(self, curr_root):
        if curr_root == None:
            return 0
        else:
            left = self.subtree_max(curr_root.left)
            right = self.subtree_max(curr_root.right)
            curr_dat = curr_root.data
            return max(left, right, curr_dat)

    def leaves_list(self):
        """
        :return: list of all leaves
        """
        if self.is_empty():
            raise Empty("LinkedBinaryTree is empty")
        else:
            return self.leaves_list_helper(self.root)

    def leaves_list_helper(self, curr_root):
        if curr_root.left == None and curr_root.right == None:
            return [curr_root.data]
        elif curr_root.left == None:
            return self.leaves_list_helper(curr_root.right)
        elif curr_root.right == None:
            return self.leaves_list_helper(curr_root.left)
        else:
            left = self.leaves_list_helper(curr_root.left)
            right = self.leaves_list_helper(curr_root.right)
            return left + right

    def is_height_balanced(self):
        """
        Returns True if the (for the entire tree) the difference in the height between
        its left child and right child is <= 1
        """
        return self.is_height_balanced_helper(self.root)[1]

    def is_height_balanced_helper(self, curr_root):
        if curr_root == None:
            return (0, True)
        else:
            left = self.is_height_balanced_helper(curr_root.left)
            right = self.is_height_balanced_helper(curr_root.right)
            is_balanced = abs(left[0] - right[0]) <= 1
            max_path = max(left[0], right[0])
            return (max_path + 1, (is_balanced and left[1] and right[1]))

    def iterative_inorder(self):
        """
        Inorder generator without recursion
        """
        current = self.root

        while current:
            if not current.left:
                yield current.data
                current = current.right
            else:
                tether = current.left
                while tether.right and tether.right != current:
                    tether = tether.right
                if not tether.right:
                    tether.right = current
                    current = current.left
                else:
                    tether.right = None
                    yield current.data
                    current = current.right

def create_subtree(data, left_child = None, right_child = None):
    subtree_root = LinkedBinaryTree.Node(data, None, left_child, right_child)
    if left_child:
        left_child.parent = subtree_root
    if right_child:
        right_child.parent = subtree_root
    return subtree_root