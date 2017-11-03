"""
Author: Jonny Sassoon
Program: Recursion intro
This program contains a series of recursive functions
"""

def print_triangle(n):
    """
    Prints a right triangle of height n
    """
    if n == 0:
        return
    print_triangle(n-1)
    print(n*"*")

def print_opposite_triangles(n):
    """
    Prints a right triangle mirrored on top of another right triangle
    """
    if n == 0:
        return
    print(n*"*")
    print_opposite_triangles(n-1)
    print(n*"*")

def print_ruler(n):
    """
    Prints an imitation ruler according to its line structure
    """
    if n == 0:
        return
    else:
        print_ruler(n-1)
        print('-'*n)
        print_ruler(n-1)

def list_min(lst, low, high):
    """
    Returns the int in lst from the interval [low, high]
    :param lst: List of ints
    :return: smallest number
    """
    if low == high:
        return lst[low]
    else:
        if lst[low] >= lst[high]:
            return list_min(lst, low+1, high)
        else:
            return list_min(lst, low, high-1)

def count_lowercase(s, low, high):
    """
    Returns the number of lowercase letters in a string over a given interval [low, high]
    :param s: input string
    :return: number of lowercase
    """
    if low == high:
        if s[low].islower():
            return 1
        else:
            return 0
    else:
        if s[high].islower():
            return count_lowercase(s, low, high-1)+1
        else:
            return count_lowercase(s, low, high-1)


def is_amount_of_lowercase_even(s, low, high):
    """
    Checks if the number of lowercase letters of a string in interval [low, high] is even
    :param s: input string
    :return: bool
    """
    if low == high:
        return s[high].islower() % 2 == 0
    else:
        test = is_amount_of_lowercase_even(s, low, high-1) + s[high].islower()
        return (test+1) % 2 == 0

def appearances(s, low, high):
    """
    Counts all the appearances of letters in a string over interval [low, high]
    :param s: input string
    :return: Dictionary
    """
    if low == high:
        return {s[low]:1}
    else:
        d = appearances(s, low, high-1)
        if s[high] not in d:
            d[s[high]] = 1
        else:
            d[s[high]] += 1
        return d

def flat_list(nested_list, low, high):
    """
    Given a nested list, the function flattens the list of the interval [low, high] and returns it
    :param nested_list: multi-D list
    :return: 1-D list
    """
    res = []
    if isinstance(nested_list, int):
        res += [nested_list]
    else:
        for i in range(low, high+1):
            if(isinstance(nested_list[i], int)):
                res += flat_list(nested_list[i], 0, 0)
            else:
                res += flat_list(nested_list[i], 0, len(nested_list[i])-1)
    return res