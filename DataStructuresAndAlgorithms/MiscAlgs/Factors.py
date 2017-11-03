"""
Author: Jonny Sassoon
Program: Factors
A generator that yields all factors of a number in increasing order
"""
def factors(num):
	for i in range(1, int(num**.5)):
		if num % i == 0:
			yield i
	for i in range(int(num**.5), 0, -1):
		if num % i == 0:
			yield num//i