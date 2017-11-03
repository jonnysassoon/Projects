"""
Author: Jonny Sassoon
Program: k Largest Elements
This function returns a list of the k largest elements in an inputted list of ints
"""
from DataStructures.PriorityQueue import ArrayMinHeap

def k_largest_elements(lst, k):
	pq = ArrayMinHeap()
	for i in range(len(lst)):
		if i <= k-1:
			pq.insert(lst[i])
		elif lst[i] > pq.min()[0]: #priority of tuple
				pq.delete_min()
				pq.insert(lst[i])
	res_lst = []
	for j in range(1, len(pq.data)):
		res_lst.append(pq.data[j].priority)
	return res_lst