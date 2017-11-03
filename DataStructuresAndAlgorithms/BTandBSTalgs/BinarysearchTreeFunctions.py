from DataStructures.BinarySearchTreeMap import BinarySearchTreeMap

def create_chain_bst(n):
    """
    Creates a Binary Search Tree with a chain of nodes from 1 to n
    :return: BinarySearchTreeMap
    """
    bst = BinarySearchTreeMap()
    for i in range(1, n+1):
        bst.subtree_insert(i)
    return bst

def create_complete_bst(n):
    """
    Creates a balanced Binary Search Tree with nodes from 1 to n
    :return: BinarySearchTreeMap
    """
    bst = BinarySearchTreeMap()
    add_items(bst, 1, n)
    return bst

def add_items(bst, low, high):
    if low == high:
        bst.subtree_insert(low)
    else:
        root_dat = (low+high)//2
        bst.subtree_insert(root_dat)
        bst.add_items(bst, low, root_dat-1)
        bst.add_items(bst, root_dat+1, high)


def find_min_abs_difference(bst):
    """
    Returns the minimum difference between the value of any two nodes in the Binary Search Tree
    """
    absolute_min = None
    previous = None
    for node_tuple in bst:
        if not previous:
            previous = node_tuple[0]
        else:
            temp_min = abs(previous - node_tuple[0])
            if not absolute_min:
                absolute_min = temp_min
            elif temp_min < absolute_min:
                absolute_min = temp_min
            previous = node_tuple[0]
    return absolute_min

def fca_bst(bst, node_a, node_b):
    """
    Finds the first common ancestor of two nodes in a Binary Search Tree
    :param bst: BinarySearchTreeMap
    :param node_a: BinarySearchTreeMap.node
    :param node_b: BinarySearchTreeMap.node
    :return: BinarySearchTreeMap.node
    """
    start = bst.root
    key_a = node_a.item.key
    key_b = node_b.item.key
    joint = (key_a < start.item.key and key_b < start.item.key) or (key_a > start.item.key and key_b > start.item.key)
    while joint:
        if key_a < start.item.key:
            start = start.left
        else:
            start = start.right

    return start