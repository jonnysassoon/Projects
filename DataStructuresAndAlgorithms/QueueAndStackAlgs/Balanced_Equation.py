"""
Author: Jonny Sassoon
Program: Balanced Equation
This function checks if an inputted equation string has balanced parentheses, brackets, and braces
"""
from DataStructures.ArrayStack import ArrayStack

def balanced_equation(input_st):
    symbol_stack = ArrayStack()
    open_symb = ['{', '(', '[']
    close_symb = {'}': '{', ')': '(', ']': '['}
    for symb in input_st:
        if symb in open_symb:
            symbol_stack.push(symb)
        else:
            if symbol_stack.is_empty():
                return False
            top = symbol_stack.pop()
            if top != close_symb[symb]:
                return False
    return symbol_stack.is_empty()