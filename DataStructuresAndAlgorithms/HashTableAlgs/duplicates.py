"""
Author: Jonny Sassoon
Program: duplicates
This function finds and returns a list of all duplicate elements in a list in Linear time
"""
def find_duplicates(lst):
	vals = dict()
	dups = []
	for i in lst:
		vals[i] = 0
	for i in lst:
		if vals[i] == 0:
			vals[i] += 1
		elif vals[i] == 1:
			dups.append(i)
			vals[i] += 1
	return dups