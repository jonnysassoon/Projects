"""
Author: Jonny Sassoon
Program: Polynomial
Implementation of a Polynomial class
"""
class Polynomial:
    def __init__(self, coefficients):
        """
        :param coefficients: List of ints
        """
        self.coefficients = coefficients

    def __str__(self):
        length = len(self.coefficients)
        equation = ''
        c = self.coefficients
        for i in range(length):
            if i != 0:
                if c[i] > 0:
                    equation += '+'
            if c[i] != 0:
                if c[i] == -1 and i != length - 1:
                    equation += '-'
                elif not (c[i] == 1 and i != length - 1):
                    equation += str(c[i])
                if i != length - 1:
                    equation += 'x'
                    if length - i > 2:
                        equation += '^' + str(length - i - 1)
        return equation

    def easy_eval(self, x):
        """
        evaluates a polynomial at some x
        """
        answer = 0
        for i in range(len(self.coefficients)):
            coef = self.coefficients[i]
            degree = len(self.coefficients) - (i+1)
            answer += x ** degree * coef
        return answer

    def __add__(self, other):
        """
        :param other: Polynomial
        :return: Polynomial
        """
        self.coefficients = self.coefficients[::-1]
        other = other.coefficients[::-1]
        if len(other) < len(self.coefficients):
            smaller = other
            new = self.coefficients.copy()
        else:
            smaller = self.coefficients
            new = other.copy
        for i in range(len(smaller)):
            new[i] += smaller[i]

        self.coefficients = self.coefficients[::-1]
        other = other[::-1]
        return Polynomial(new[::-1])

    def __mul__(self, other):
        other = other.coefficients[::-1]
        new = [0] * (len(self.coefficients) + len(other) - 1)
        self.coefficients = self.coefficients[::-1]
        for i in range(len(other)):
            for j in range(len(self.coefficients)):
                new[i + j] += other[i] * self.coefficients[j]

        other = other[::-1]
        self.coefficients = self.coefficients[::-1]
        return Polynomial(new[::-1])