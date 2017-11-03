"""
Author: Jonny Sassoon
Program: Compact String
Implementation of CompactString class that takes a string and compresses
it into a DLL where each node contains a tuple of the letter and the number
of times it is used consecutively
"""
from DataStructures.DoublyLinkedList import DoublyLinkedList

class CompactString:
	def __init__(self, orig_str = ''):
		self.data = DoublyLinkedList()
		count = 1
		for i in range(len(orig_str)):
			if not i == len(orig_str)-1:
				if orig_str[i+1] != orig_str[i]:
					self.data.add_last((orig_str[i], count))
					count = 1
				else:
					count += 1
			else:
				self.data.add_last((orig_str[i], count))

	def __add__(self, other): # concatination of two CompactStrings
		new_Cstring = CompactString()
		for data in self.data:
			new_Cstring.data.add_last(data)
		for data in other.data:
			new_Cstring.data.add_last(data)
		return new_Cstring

	def __lt__(self, other): # returns true if self is lexicographically less than other
		data1 = self.data.first_node()
		data2 = other.data.first_node()
		while not (data1 == self.data.trailer or data2 == other.data.trailer):
			if data1.data[0] < data2.data[0]:
				return True
			elif data1.data[0] == data2.data[0]:
				if data1.data[1] < data2.data[1]:
					if data1.next  == self.data.trailer:
						return True
					if data1.next.data[0] < data2.data[0]:
						return True
					return False
				elif data1.data[1] > data2.data[1]:
					if data2.next == other.data.trailer:
						return False
					if data2.next.data[0] > data1.data[0]:
						return True
					return False
				else:
					data1 = data1.next
					data2 = data2.next
			else:
				return False
		if data1 == self.data.trailer and not data2 == other.data.trailer:
			return True
		return False

	def __le__(self, other):
		data1 = self.data.first_node()
		data2 = other.data.first_node()
		while not (data1 == self.data.trailer or data2 == other.data.trailer):
			if data1.data[0] < data2.data[0]:
				return True
			elif data1.data[0] == data2.data[0]:
				if data1.data[1] < data2.data[1]:
					if data1.next  == self.data.trailer:
						return True
					if data1.next.data[0] < data2.data[0]:
						return True
					return False
				elif data1.data[1] > data2.data[1]:
					if data2.next == other.data.trailer:
						return False
					if data2.next.data[0] > data1.data[0]:
						return True
					return False
				else:
					data1 = data1.next
					data2 = data2.next
			else:
				return False
		if data2 == other.data.trailer and not data1 == self.data.trailer:
			return False
		return True

	def __gt__(self, other):
		return not (self <= other)

	def __ge__(self, other):
		return not (self < other)

	def __str__(self):
		s_ls = []
		for i in self.data:
			s_ls.append(i[0] * i[1])
		st = ''.join(s_ls)
		return st

	def __repr__(self):
		return str(self)