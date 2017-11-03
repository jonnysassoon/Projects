"""
Author: Jonny Sassoon
Program: two_sum
This function determines if two ints in a sorted list sum to a target
"""
def two_sum(srt_lst, target):
	a = 0
	b = len(srt_lst)-1
	while a != b:
		if srt_lst[a]+srt_lst[b] < target:
			a += 1
		elif srt_lst[a]+srt_lst[b] > target:
			b -= 1
		else:
			return (a, b)
	return None