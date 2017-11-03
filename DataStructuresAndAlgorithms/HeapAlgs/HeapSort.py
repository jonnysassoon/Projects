"""
Author: Jonny Sassoon
Program Heap Sort
Heap sort function
"""
from DataStructures.PriorityQueue import ArrayMinHeap

def heap_sort(lst):
    n = len(lst)
    heap = ArrayMinHeap(lst, [None] * n)
    res_lst = []
    for i in range(n):
        (priority, value) = heap.delete_min()
        res_lst.append(priority)
    return res_lst