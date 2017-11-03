"""
Author: Jonny Sassoon
Program: permutations
This function returns a list of all permutations of an inputted
list in the interval [low, high] leaving everything else untouched
"""
def permutations(lst, low, high):
	if low == high:
		return [lst]
	elif low+1 == high:
		l1 = lst
		l2 = lst[:low]+[lst[high]]+[lst[low]]
		if high != len(lst)-1:
			l2 += lst[high+1:]
		return [l1, l2]
	else:
		final = []
		for i in range(len(lst)):
			if i >= low and i <= high:
				rest = lst.copy()
				hold = rest.pop(i)
				past = permutations(rest, low, high-1)
				for sub in past:
					sub.insert(low, hold)
					final.append(sub)
		return final