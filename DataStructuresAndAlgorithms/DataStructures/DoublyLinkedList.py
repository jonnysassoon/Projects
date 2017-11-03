"""
Author: Jonny Sassoon
Program: DoublyLinkedList
Implementation of DoublyLinkedList Data Type including additional, non-trivial methods (sorting, reversing, removing)
"""
class Empty(Exception):
    pass


class DoublyLinkedList:
    class Node:
        def __init__(self, data=None, prev=None, next=None):
            self.data = data
            self.prev = prev
            self.next = next

        def disconnect(self):
            self.data = None
            self.prev = None
            self.next = None

    def __init__(self):
        self.header = DoublyLinkedList.Node()
        self.trailer = DoublyLinkedList.Node()
        self.header.next = self.trailer  # header is the starter node, next is the 'next' component of the header node.
        self.trailer.prev = self.header
        self.size = 0

    def __len__(self):
        return self.size

    def __iter__(self):
        if self.is_empty():
            return
        cursor = self.first_node()
        while cursor != self.trailer:
            yield cursor.data
            cursor = cursor.next

    def __str__(self):
        return '[' + '<-->'.join([str(item) for item in self]) + ']'

    def __repr__(self):
        return str(self)

    def is_empty(self):
        return len(self) == 0

    def first_node(self):
        if len(self) == 0:
            raise Exception
        return self.header.next

    def last_node(self):
        if len(self) == 0:
            raise Exception
        return self.trailer.prev  # tail node. prev is a reference to the node right before the tail

    def add_after(self, node, data):  # whole function is O(1) time
        pred = node
        succ = node.next
        new_node = DoublyLinkedList.Node(data, pred, succ)  # utilizing default variables.
        pred.next = new_node
        succ.prev = new_node
        self.size += 1
        return new_node

    def add_first(self, data):
        return self.add_after(self.header, data)

    def add_last(self, data):
        return self.add_after(self.trailer.prev, data)

    def add_before(self, node, data):
        return self.add_after(node.prev, data)

    def delete(self, node):  # make node before it look to the one after it and vice versa works just like pop()
        value = node.data
        pred = node.prev
        succ = node.next
        pred.next = succ
        succ.prev = pred
        self.size -= 1
        node.disconnect()
        return value

    # NONTRIVIAL METHODS:
    def remove_all(self, elem):
        cursor = self.first_node()
        while cursor != self.last_node():
            if cursor.data == elem:
                new_cursor = cursor.next
                self.delete(cursor)
                cursor = new_cursor
            else:
                cursor = cursor.next

    def max_in_sublist(self, curr_head):
        if curr_head is self.last_node():
            return curr_head.data
        else:
            rest_max = max_in_sublist(self, curr_head.next)
            if curr_head.data > rest_max:
                rest_max = curr_head.data
            return rest_max

    def max(self):
        if self.is_empty():
            raise Empty("No max in empty list")
        self.max_in_sublist(self.first_node())

    def split_list(self, left_lst_size):  # Returns a tuple of the halved lists
        left_lst = DoublyLinkedList()
        right_lst = DoublyLinkedList()
        right_lst_size = len(self) - left_lst_size
        cursor = self.first_node
        for i in range(left_lst_size):
            left_lst.add_last(cursor.data)
            cursor = cursor.next
        for j in range(right_lst_size):
            right_lst.add_last(cursor.data)
            cursor = cursor.next
        return (left_lst, right_lst)

    def merge(self, other):
        res_lst = DoublyLinkedList()
        cursor1 = self.first_node()
        cursor2 = other.first_node()
        while (cursor1 is not self.trailer and
                       cursor2 is not other.trailer):
            if (cursor1.data < cursor2.data):
                res_lst.add_last(cursor1.data)
                cursor1 = cursor1.next
            else:
                res_lst.add_last((cursor2.data))
                cursor2 = cursor2.next

            while (cursor1 is not self.trailer):
                res_lst.add_last(cursor1.data)
                cursor1 = cursor1.next

            while (cursor2 is not other.trailer):
                res_lst.add_last(cursor2.data)
                cursor2 = cursor2.next

            return res_lst

    def merge_sort(self):
        if len(self) <= 1:
            return
        else:
            (left_lst, right_lst) = self.split_list(len(self) // 2)
            left_lst.merge_sort()
            right_lst.merge_sort()
            res = left_lst.merge(right_lst)
            cursor = self.first_node()
            for item in res:
                cursor.data = item
                cursor = cursor.next

    def reverse(self):
        if self.is_empty():
            raise Empty("DoublyLinkedList is empty")
        else:
            self.reverse_sublist(self.last_node())

    def reverse_sublist(self, start_node):
        if len(self) == 1:
            return
        else:
            prev = start_node.prev
            val = self.delete(start_node)
            self.reverse_sublist(prev)
            self.add_first(val).data
