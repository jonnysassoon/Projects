"""
Author: Jonny Sassoon
Program: Stack
Implementation of LIFO Data Structure
"""
class Empty(Exception):
    pass


class ArrayStack:
    def __init__(self):
        self.data = []

    def __len__(self):
        return len(self.data)

    def is_empty(self):
        return (len(self) == 0)

    def push(self, element):
        self.data.append(element)

    def pop(self):
        if self.is_empty():
            raise Empty("Stack is empty.")
        return self.data.pop()

    def top(self):
        if self.is_empty():
            raise Empty("Stack is empty.")
        return self.data[-1]