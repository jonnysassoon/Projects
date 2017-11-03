"""
Author: Jonny Sassoon
Program: Unsorted Array Map
Implementation of basic Map Class using an array of Tuples
"""
class UnsortedArrayMap:

	class Item:
		def __init__(self, key, value=None):
			self.key = key
			self.value = value

	def __init__(self):
		self.table = []

	def __len__(self):
		return len(self.table)

	def is_empty(self):
		return len(self) == 0

	def __getitem__(self, key):
		for item in self.table:
			if item.key == key:
				return item.value
		raise KeyError(str(key) + "not found")

	def __setitem__(self, key, value):
		for item in self.table:
			if item.key == key:
				item.value = value
				return
		new = UnsortedArrayMap.Item(key, value)
		self.table.append(new)

	def __delitem__(self, key):
		for j in range(len(self.table)):
			if self.table[j].key == key:
				self.table.pop(j)
				return
		raise KeyError(str(key) + "not found")

	def __iter__(self):
		for item in self.table:
			yield item.key