"""
Author: Jonny Sassoon
Program: MaxStack
Implementaiton of MaxStack: traditional stack but you have constant access
to the element with the highest value in the stack at any time
"""
from DataStructures.ArrayStack import ArrayStack
class Empty(Exception):
	pass

class MaxStack:
	def __init__(self):
		self.data = ArrayStack()
		self.max_val = 0

	def is_empty(self):
		return len(self.data) == 0

	def __len__(self):
		return len(self.data)

	def push(self, e):
		if e > self.max_val:
			self.max_val = e
		tup = (e, self.max_val)
		self.data.push(tup)

	def top(self):
		if self.is_empty():
			raise Empty("MaxStack is empty")
		return self.data.top()[0]

	def pop(self):
		if self.is_empty():
			raise Empty("MaxStack is empty")
		top = self.top()
		self.data.pop()
		return top

	def max(self):
		if self.is_empty():
			raise Empty("MaxStack is empty")
		return self.data.top()[1]
