"""
Author: Jonny Sassoon
Program: Binary Search Tree Map
Implementation of Binary Tree with additional mapping quality.
"""
class BinarySearchTreeMap:
    class item:
        def __init__(self, key, value=None):
            self.key = key
            self.value = value

    class node:
        def __init__(self, item, parent=None, left=None, right=None):
            self.right = right
            self.left = left
            self.parent = parent
            self.item = item
            self.size = 1

        def disconnect(self):
            self.item = None
            self.left = None
            self.right = None
            self.parent = None

    def __init__(self):
        self.root = None
        self.size = 0

    def __len__(self):
        return self.size

    def __iter__(self):
        for node in self.inorder():
            yield (node.item.key, node.item.value)

    def is_empty(self):
        return len(self) == 0

    def get_ith_smallest(self, i):
        """
        Returns the ith smallest key in the BinarySearchTree
        """
        if i > len(self):
            raise IndexError("index " + str(i) + "out of range")
        else:
            return self.get_ith_smallest_helper(self.root, i)

    def get_ith_smallest_helper(self, curr_root, i):
        if curr_root.left == None:
            left_size = 0
        else:
            left_size = curr_root.left.size
        if i <= left_size:
            return self.get_ith_smallest_helper(curr_root.left, i)
        elif i == left_size + 1:
            return curr_root.item.key
        else:
            return self.get_ith_smallest_helper(curr_root.right, i - (left_size + 1))

    # raises exception if key doesn't exist
    def __getitem__(self, key):
        node = self.subtree_find(self.root, key)
        if node:
            return node.item.value
        else:
            raise KeyError(str(key) + ' not found')

    # returns None if key doesn't exist or the node containing key if key is in tree
    def subtree_find(self, curr_root, key, reduce=False):
        if curr_root == None:
            return None
        else:
            if reduce:
                curr_root.size -= 1
            if curr_root.item.key == key:
                return curr_root
            elif key < curr_root.item.key:
                return self.subtree_find(curr_root.left, key, reduce)
            else:
                return self.subtree_find(curr_root.right, key, reduce)

    # updates value associated with key if key exists; otherwise creates new
    def __setitem__(self, key, value):
        node = self.subtree_find(self.root, key)
        if node:
            node.item.value = value
        else:
            self.subtree_insert(key, value)

    def subtree_insert(self, key, value=None):
        entry = BinarySearchTreeMap.item(key, value)
        new_node = BinarySearchTreeMap.node(entry)

        if self.root == None:
            self.root = new_node
        else:
            parent = self.root
            if parent.item.key > key:
                parent.size += 1
                curr_root = self.root.left
            else:
                parent.size += 1
                curr_root = self.root.right
            while curr_root:
                parent = curr_root
                if key < curr_root.item.key:
                    parent.size += 1
                    curr_root = curr_root.left
                else:
                    parent.size += 1
                    curr_root = curr_root.right

            new_node.parent = parent
            if key < parent.item.key:
                parent.left = new_node
            else:
                parent.right = new_node

        self.size += 1

    # raises exception if key is not in tree
    def __delitem__(self, key):
        node = self.subtree_find(self.root, key)
        if not node:
            raise KeyError(str(key) + "not found")
        else:
            self.subtree_delete(self.root, key)

    # assumes key is in tree
    def subtree_delete(self, curr_root, key):
        node_to_delete = self.subtree_find(curr_root, key, True)
        value = node_to_delete.item.value
        num_children = self.num_of_children(node_to_delete)
        if node_to_delete == self.root:
            if num_children == 0:
                node_to_delete.disconnect()
                self.root = None
                self.size = 0
            elif num_children == 1:
                if self.root.left:
                    child = self.root.left
                else:
                    child = self.root.right
                self.root = child
                self.size -= 1
                node_to_delete.disconnect()
            else:
                max_in_left = self.max_in_subtree(node_to_delete.left)
                node_to_delete.item = max_in_left.item
                self.subtree_delete(node_to_delete.left, max_in_left.item.key)
        else:
            if num_children == 0:
                parent = node_to_delete.parent
                if node_to_delete == parent.right:
                    parent.right = None
                else:
                    parent.left = None
                node_to_delete.disconnect()
                self.size -= 1
            elif num_children == 1:
                parent = node_to_delete.parent
                if node_to_delete.left:
                    child = node_to_delete.left
                else:
                    child = node_to_delete.right
                if parent.left == node_to_delete:
                    parent.left = child
                else:
                    parent.right = child
                node_to_delete.disconnect()
            else:
                max_in_left = self.max_in_subtree(node_to_delete.left)
                node_to_delete.item = max_in_left.item
                self.subtree_delete(node_to_delete.left, max_in_left.item.key)

        return value

    def num_of_children(self, node):
        count = 0
        if not (node.left or node.right):
            pass
        elif node.right and node.left:
            count += 2
        else:
            count += 1

        return count

    def max_in_subtree(self, curr_root):
        node = curr_root
        while node.right != None:
            node = node.right
        return node

    def inorder(self):
        yield from self.subtree_inorder(self.root)

    def subtree_inorder(self, curr_root):
        if curr_root == None:
            pass
        else:
            yield from self.subtree_inorder(curr_root.left)
            yield curr_root
            yield from self.subtree_inorder(curr_root.right)

    def insert(self, key, value):
        self.recursive_subtree_insert(self.root, key, value)

    def recursive_subtree_insert(self, curr_root, key, value):
        entry = BinarySearchTreeMap.item(key, value)
        new_node = BinarySearchTreeMap.node(entry)
        node_in_tree = self.subtree_find(curr_root, key)
        if node_in_tree:
            node_in_tree.item.value = value
        else:
            if not curr_root.left and not curr_root.right:
                if key <= curr_root.item.key:
                    curr_root.left = new_node
                else:
                    curr_root.right = new_node
            elif not curr_root.left:
                if key <= curr_root.item.key:
                    curr_root.left = new_node
                else:
                    return self.recursive_subtree_insert(curr_root.right, key, value)
            elif not curr_root.right:
                if key <= curr_root.item.key:
                    return self.recursive_subtree_insert(curr_root.left, key, value)
                else:
                    curr_root.right = new_node
            else:
                if key <= curr_root.item.key:
                    return self.recursive_subtree_insert(curr_root.left, key, value)
                else:
                    return self.recursive_subtree_insert(curr_root.right, key, value)