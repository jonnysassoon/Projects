"""
Author: Jonny Sassoon
Program: Prefix to Postfix
This function (along with its helper function) converts a prefix expression
string into a postfix expression string
"""
def prefix_to_postfix(prefix_exp_st):
	prefix_exp_lst = []
	symb = ''
	for aChar in prefix_exp_st: # organize into a list of all of the components
		if aChar == ' ':
			prefix_exp_lst.append(symb)
			symb = ''
		else:
			symb += aChar
	prefix_exp_lst.append(symb)
	postfix_exp_lst = prefix_to_postfix_helper(prefix_exp_lst, 0)
	postfix_exp_st = ' '.join(postfix_exp_lst)
	return postfix_exp_st

def prefix_to_postfix_helper(prefix_exp_lst, start_pos):
	if prefix_exp_lst[start_pos].isdigit():
		return [prefix_exp_lst[start_pos]]
	else:
		operator = prefix_exp_lst[start_pos]
		start_pos += 1
		operand_1 = prefix_to_postfix_helper(prefix_exp_lst, start_pos)
		start_pos += len(operand_1)
		operand_2 = prefix_to_postfix_helper(prefix_exp_lst, start_pos)
		return operand_1 + operand_2 + [operator]