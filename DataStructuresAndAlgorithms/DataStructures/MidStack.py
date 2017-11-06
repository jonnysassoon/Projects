"""
Author: Jonny Sassoon
Program: MidStack
Implementation of MidStack: traditional stack, but you can push into the middle of the
stack as well as onto the top
"""
from DoubleEndedQueue import ArrayDeque

from DataStructures.ArrayStack import ArrayStack


class Empty(Exception):
	pass

class MidStack:
	def __init__(self):
		self.front_to_mid = ArrayStack()
		self.back_end = ArrayDeque()
		self.size = 0

	def is_empty(self):
		return self.size == 0

	def __len__(self):
		return self.size

	def push(self, e):
		if len(self.back_end) > len(self.front_to_mid):
			switch = self.back_end.delete_first()
			self.front_to_mid.push(switch)
		self.back_end.add_last(e)
		self.size += 1

	def top(self):
		if self.is_empty():
			raise Empty("MidStack is empty")
		if self.back_end.is_empty():
			return self.front_to_mid.top()
		return self.back_end.last()

	def pop(self):
		if self.is_empty():
			raise Empty("MidStack is empty")
		if len(self.back_end) < len(self.front_to_mid):
			switch = self.front_to_mid.pop()
			self.back_end.add_first(switch)
		val = self.back_end.delete_last()
		self.size -= 1
		return val

	def mid_push(self, e):
		if len(self.back_end) > len(self.front_to_mid):
			self.front_to_mid.push(e)
		else:
			self.back_end.add_first(e)
		self.size += 1
