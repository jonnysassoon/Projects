"""
Author: Jonny Sassoon
Program: Linked Queue
Implementation of Queue Data Type using Linked List
"""

from DataStructures.DoublyLinkedList import DoublyLinkedList


class Empty(Exception):
    pass


class LinkedQueue:
    def __init__(self):
        self.data = DoublyLinkedList()
        self.front = self.data.header.next
        self.size = 0

    def __len__(self):
        return self.size

    def is_empty(self):
        return len(self) == 0

    def enqueue(self, val):
        self.data.add_last(val)
        self.front = self.data.header.next
        self.size += 1

    def dequeue(self):
        if (self.is_empty()):
            raise Empty("Queue is empty")
        val = self.front.data
        self.data.delete(self.front)
        self.front = self.data.header.next
        self.size -= 1
        return val

    def first(self):
        if self.is_empty():
            raise Empty("Queue is empty")
        return self.front.data