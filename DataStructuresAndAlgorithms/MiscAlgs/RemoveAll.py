"""
Author: Jonny Sassoon
Program: Remove All
This function removes all vals from a lst in linear time
"""
def remove_all(lst, val):
	if val not in lst:
		raise ValueError("Value is not present")
	for i in range(len(lst)):
		if lst[i] == val:
			lst[i] = None
	shift = 0	
	for j in range(len(lst)):
		if lst[j] is None:
			shift += 1
		else:
			lst[j-shift] = lst[j]
	for k in range(shift):
		lst.pop()