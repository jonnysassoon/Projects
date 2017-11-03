"""
Author: Jonny Sassoon
Program: Eval Postfix
This function evaluates a postfix mathematical expression using a stack
"""
from DataStructures.ArrayStack import ArrayStack

def eval_postfix_eq(exp_str):
    exp = exp_str.split()
    arg_stack = ArrayStack()
    for token in exp:
        if token.isdigit():
            arg_stack.push(float(token))
        else:
            arg_2 = arg_stack.pop()
            arg_1 = arg_stack.pop()
            res = None
            if token == '+':
                res = arg_1 + arg_2
            elif token == '-':
                res = arg_1 - arg_2
            elif token == '*':
                res = arg_1 * arg_2
            elif token == '/':
                res = arg_1 / arg_2
            arg_stack.push(res)
    return arg_stack.pop()