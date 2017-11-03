"""
Author: Jonny Sassoon
Program: Copy Functions
This program includes the implementation of both a shallow copy and
a recursive deep copy of a linked list
"""

from DataStructures.DoublyLinkedList import DoublyLinkedList

def copy_linked_list(lnkd_list): # shallow copy
	new = DoublyLinkedList()
	for elem in lnkd_list:
		new.add_last(elem)

	return new

def deep_copy_linked_list(lnk_lst): # deep copy
	return deep_copy_sub_linked_list(lnk_lst, lnk_lst.first_node())

def deep_copy_sub_linked_list(lnk_lst, node): # recursive helper
	if node == lnk_lst.trailer.prev:
		if isinstance(node.data, int):
			new = DoublyLinkedList()
			new.add_first(node.data)
			return new
		else:
			return deep_copy_sub_linked_list(node.data, node.data.first_node())
	else:
		others = deep_copy_sub_linked_list(lnk_lst, node.next)
		if not isinstance(node.data, int):
			copied_dat = deep_copy_sub_linked_list(node.data, node.data.first_node())
			others.add_first(copied_dat)
		else:
			others.add_first(node.data)
		return others
