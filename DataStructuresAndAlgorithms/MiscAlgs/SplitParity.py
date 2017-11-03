"""
Author: Jonny Sassoon
Program: split_parity
This function takes a list of ints and reorders it in linear time
so that all odd numbers appear first
"""
def split_parity(lst):
    low = 0
    high = len(lst)-1
    while high > low:
        if lst[low]%2 != 0:
            low += 1
        if lst[high]%2 == 0:
            high -= 1
        elif lst[low]%2 == 0 and lst[high]%2 != 0:
            lst[low], lst[high] = lst[high], lst[low]
            low += 1
            high -= 1
