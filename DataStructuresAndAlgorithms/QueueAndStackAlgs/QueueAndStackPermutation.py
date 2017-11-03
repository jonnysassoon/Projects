"""
Author: Jonny Sassoon
Program: Permuations
This function returns all permutations of a list using a stack and a queue
"""
from DataStructures.ArrayQueue import ArrayQueue
from DataStructures.ArrayStack import ArrayStack


def permutations(lst):
    holder = ArrayStack()
    perms_set = ArrayQueue()
    for elem in lst:
        holder.push(elem)
    for i in range(len(lst)):
        curr = holder.pop()
        if i == 0:
            perms_set.enqueue([curr])
        else:
            for j in range(len(perms_set)):
                sub = perms_set.dequeue()
                for k in range(len(sub) + 1):
                    temp = sub.copy()
                    temp.insert(k, curr)
                    perms_set.enqueue(temp)
    final_lst = []
    while not perms_set.is_empty():
        final_lst.append(perms_set.dequeue())

    return final_lst