"""
Author: Jonny Sassoon
Program: Queue from Stacks
This is a Queue class implementation using two stacks as data members
"""

from DataStructures.ArrayStack import ArrayStack

class Empty(Exception):
	pass

class Queue:
	def __init__(self):
		self.enqueue_stack = ArrayStack()
		self.dequeue_stack = ArrayStack()
		self.size = 0

	def __len__(self):
		return self.size

	def is_empty(self):
		return len(self) == 0

	def enqueue(self, e):
		self.enqueue_stack.push(e)
		self.size += 1

	def first(self):
		if self.is_empty():
			raise Empty('Queue is empty.')
		if self.dequeue_stack.is_empty():
			return self.enqueue_stack.data[0]
		return self.dequeue_stack.top()

	def dequeue(self): # dequeue runs in linear time :(
		if self.is_empty():
			raise Empty('Queue is empty.')
		if self.dequeue_stack.is_empty():
			while not self.enqueue_stack.is_empty():
				item = self.enqueue_stack.pop()
				self.dequeue_stack.push(item)
		self.size -= 1
		return self.dequeue_stack.pop()

	def __str__(self):
		s_ls = []
		for i in range(len(self.enqueue_stack)-1, -1, -1):
			s_ls.append(self.enqueue_stack.data[i])
		for i in range(len(self.dequeue_stack)):
			s_ls.append(self.dequeue_stack.data[i])
		return str(s_ls)

	def __repr__(self):
		return str(self)