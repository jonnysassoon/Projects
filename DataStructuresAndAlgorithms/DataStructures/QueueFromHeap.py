"""
Author: Jonny Sassoon
Program: Queue from MinHeap
Implementation of FIFO Data Structure using a Priority Queue
"""
from DataStructures.PriorityQueue import ArrayMinHeap

class Empty(Expection):
	pass

class Queue:
	def __init__(self):
		self.data = ArrayMinHeap()
		self.size = 0
		self.FIFO_order = 0

	def __len__(self):
		return self.size

	def is_empty(self):
		return len(self) == 0

	def enqueue(self, elem):
		self.data.insert(self.FIFO_order, elem)
		self.FIFO_order += 1
		self.size += 1

	def dequeue(self):
		if self.is_empty():
			raise Empty("Queue is empty.")
		tup = self.data.delete_min()
		self.size -= 1
		return tup[1]

	def first(self):
		if self.is_empty():
			raise Empty("Queue is empty.")
		tup = self.data.min()
		return tup[1]