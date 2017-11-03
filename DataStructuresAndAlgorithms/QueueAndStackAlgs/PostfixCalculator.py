"""
Author: Jonny Sassoon
Program: Postfix Calculator
This function allows a user to input a postfix expression, and it returns the result of the expression
Users can also store values as variables and refer to those variables throughout usage
"""
from DataStructures.ArrayStack import ArrayStack


def eval_postfix_eq():
    exp_str = input("--> ")
    arg_stack = ArrayStack()
    var_assignments = dict()
    while exp_str != 'done()':
        exp = exp_str.split()
        if len(exp) == 1:
            if exp[0].isdigit():
                print(int(exp[0]))
            else:
                out = var_assignments[exp[0]]
                if isinstance(out, str):
                    out = var_assignments[out]
                if out % 1 == 0:
                    print(int(out))
                else:
                    print(out)
        else:
            for i in range(len(exp)):
                if i == 0 and exp[0].isalpha() and exp[1] == '=':
                    continue
                if exp[i] == '=':
                    continue
                elif exp[i].isdigit():
                    arg_stack.push(float(exp[i]))
                elif exp[i].isalpha():
                    arg_stack.push(exp[i])
                else:
                    arg_2 = arg_stack.pop()
                    arg_1 = arg_stack.pop()
                    if not isinstance(arg_1, float):
                        arg_1 = var_assignments[arg_1]
                    if not isinstance(arg_2, float):
                        arg_2 = var_assignments[arg_2]
                    if exp[i] == '+':
                        res = arg_1 + arg_2
                    elif exp[i] == '-':
                        res = arg_1 - arg_2
                    elif exp[i] == '*':
                        res = arg_1 * arg_2
                    else:
                        res = arg_1 / arg_2
                    arg_stack.push(res)

            if isinstance(exp[0], str) and exp[1] == '=':
                var_assignments[exp[0]] = arg_stack.pop()
                print(exp[0])
            else:
                out = arg_stack.pop()
                if out % 1 == 0:
                    print(int(out))
                else:
                    print(out)

        exp_str = input("--> ")


eval_postfix_eq()