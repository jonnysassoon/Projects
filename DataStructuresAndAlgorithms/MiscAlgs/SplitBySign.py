"""
Author: Jonny Sassoon
Program: split_by_signs
This function takes a list of ints and recursively reorders it over an
interval [low, high] such that all negative elements come before all positive elements
"""
def split_by_sign(lst, low, high):
    if high == low:
        return
    if high - low == 1: # len(interval) == 2
        if lst[low] > 0 and lst[high] < 0: # if there's a negative after a positive
            lst[low], lst[high] = lst[high], lst[low] #swap
    else:
        if lst[low] < 0 and lst[high] < 0:
            split_by_sign(lst, low+1, high)
        elif lst[low] > 0 and lst[high] > 0:
            split_by_sign(lst, low, high-1)
        elif lst[low] > 0 and lst[high] < 0: # need to swap
            lst[low], lst[high] = lst[high], lst[low]
            split_by_sign(lst, low+1, high-1)
        elif lst[low] < 0 and lst[high] > 0: # could have used else
            split_by_sign(lst, low+1, high-1)