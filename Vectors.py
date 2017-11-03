"""
Author: Jonny Sassoon
Program: Vector Class
This class is a mathematical vector that suports indexing, assignment, and vector math
"""
class Vector:
	def __init__(self, d):
		if isinstance(d, list):
			self.coords = d
		else:
			self.coords = [0]*d
		
	def __len__(self):
		return len(self.coords)
		
	def __getitem__(self, j):
		return self.coords[j]
		
	def __setitem__(self, j, val):
		self.coords[j] = val
		
	def __add__(self, other):
		if (len(self) != len(other)):
			raise ValueError("dimensions must agree")
		result = Vector(len(self))
		for j in range(len(self)):
			result[j] = self[j] + other[j]
		return result
		
	def __sub__(self, other):
		if (len(self) != len(other)):
			raise ValueError("dimensions must agree")
		result = Vector(len(self))
		for j in range(len(self)):
			result[j] = self[j] - other[j]
		return result
		
	def __neg__(self):
		result = Vector(len(self))
		for j in range(len(self)):
			result[j] = -self[j]
		return result
		
	def __mul__(self, other):
		if isinstance(other, int):
			result = Vector(len(self))
			for j in range(len(self)):
				result[j] = self[j]*other
			return result
		elif len(self) != len(other):
			raise ValueError("dimensions must agree")
		else:
			tot = 0
			for i in range(len(self)):
				tot += self[i]*other[i]
		return tot
		
	def __rmul__(self, other):
		if isinstance(other, int):
			result = Vector(len(self))
			for j in range(len(self)):
				result[j] = other*self[j]
			return result
		elif len(self) != len(other):
			raise ValueError("dimensions must agree")
		else:
			tot = 0
			for i in range(len(self)):
				tot += self[i]*other[i]
		return tot
	
	def __eq__(self, other):
		return self.coords == other.coords
		
	def __ne__(self, other):
		return not (self == other)
		
	def __str__(self):
		return '<'+ str(self.coords)[1:-1] + '>'
		
	def __rptr__(self):
		return str(self)