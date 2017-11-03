"""
Author: Jonny Sassoon
Program: Integer Class
Implementation of an Integer class that stores digits as nodes in a DLL
"""
from DataStructures.DoublyLinkedList import DoublyLinkedList

class Integer:
    def __init__(self, num_str):
        self.data = DoublyLinkedList()
        for ch in num_str:
            self.data.add_last(ch)

    def __add__(self, other):
        new = Integer('')
        if len(other.data) < len(self.data):
            smaller = other
            larger = self
        else:
            smaller = self
            larger = other
        cursor1 = smaller.data.trailer.prev
        cursor2 = larger.data.trailer.prev
        carry = 0
        for i in range(len(smaller.data)):
            num1 = int(cursor1.data)
            num2 = int(cursor2.data)
            digit = num1 + num2 + carry
            if digit >= 10:
                digit %= 10
                carry = 1
            else:
                carry = 0
            new.data.add_first(str(digit))
            cursor1 = cursor1.prev
            cursor2 = cursor2.prev
        while cursor2 != larger.data.header:
            num = int(cursor2.data)
            digit = num + carry
            if digit >= 10:
                digit %= 10
                carry = 1
            else:
                carry = 0
            new.data.add_first(str(digit))
            cursor2 = cursor2.prev
        if carry == 1:
            new.data.add_first(str(carry))
        return new

    def __mul__(self, other):
        self.data.reverse()  # reverse so it makes looping easier
        other.data.reverse()
        carry = 0
        zeroes = 0
        sub_products_holder = []
        curr = other.data.first_node()
        for dig_1 in self.data:
            sub_product = Integer('0' * zeroes)
            while curr != other.data.trailer:
                dig_2 = curr.data
                raw = int(dig_2) * int(dig_1) + carry
                if not curr.next == other.data.trailer:
                    digit = raw % 10
                    carry = raw // 10
                else:
                    digit = raw
                sub_product.data.add_first(digit)
                curr = curr.next
            curr = other.data.first_node()
            carry = 0
            zeroes += 1
            sub_products_holder.append(sub_product)
        new = Integer('0')
        for num in sub_products_holder:
            new = new + num
        self.data.reverse()  # switch back
        other.data.reverse()
        return new

    def __str__(self):
        ls = []
        for ch in self.data:
            ls.append(ch)
        st = ''.join(ls)
        return st

    def __repr__(self):
        return str(self)