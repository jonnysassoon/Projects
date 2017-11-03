"""
Author: Jonny Sassoon
Program: e Approximation
This function gives the taylor series of (n) approximation of e
"""
def e_approx(n):
	approx = 1
	denom = 1
	for i in range(1,n+1):
		denom *= i
		approx += 1/denom
		
	return approx
	
def main():
	for n in range(15):
		curr_approx = e_approx(n)
		approx_str = "{:15f}".format(curr_approx)
		print("n=", n, "approximation:", approx_str)
		
main()