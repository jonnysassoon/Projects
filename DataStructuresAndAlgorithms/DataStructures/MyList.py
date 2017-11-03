"""
Author: Jonny Sassoon
Program: MyList
This is a class that imitates the Python List implementation
"""
import ctypes  # provides low level arrays


def make_array(n):
    return (n * ctypes.py_object)()


class MyList:
    def __init__(self):
        self.data = make_array(1)  # actual structure of the list
        self.capacity = 1  # total list capacity
        self.n = 0  # current number of items in list

    def __len__(self):
        return self.n

    def __str__(self):
        s_ls = []
        for i in self:
            s_ls.append(i)
        return str(s_ls)

    def __repr__(self):
        ls = []
        for i in self.data:
            ls.append(i)
        return ls

    def __getitem__(self, index):
        if index > self.n - 1 or index < -self.n:
            raise IndexError("MyList index is out of range.")
        return self.data[index]

    def __setitem__(self, index, val):
        if index > self.n - 1 or index < -self.n:
            raise IndexError("MyList index is out of range.")
        self.data[index] = val

    def __add__(self, list2):
        new = MyList()
        new.capacity = self.capacity + list2.capacity
        new.n = self.n + list2.n
        new.data = make_array(new.capacity)
        j = 0
        for i in range(len(self) + len(list2)):
            if i <= len(self) - 1:
                new[i] = self[i]
            else:
                new[i] = list2[j]
                j += 1
        return new

    def __iadd__(self, ls):
        for i in ls:
            self.append(i)
            print(self)

    def __mul__(self, val):
        new = MyList()
        new.capacity = self.capacity * val
        new.n = self.n * val
        new.data = make_array(new.capacity)
        for i in range(new.n):
            self_ind = i % (self.n)
            new.data[i] = self[self_ind]
        return new

    def __rmul__(self, other):
        new = MyList()
        new.capacity = self.capacity * other
        new.n = self.n * other
        new.data = make_array(new.capacity)
        for i in range(new.capacity):
            ls_ind = i % (self.n)
            new.data[i] = self[ls_ind]
        return new

    def __eq__(self, other):
        try:
            len(self) == len(other)
        except TypeError:
            return False
        if type(self) != type(other):
            return False
        else:
            if len(other) != len(self):
                return False

            for i in range(len(other)):
                if other[i] != self[i]:
                    return False

        return True

    def __ne__(self, other):
        return not (self == other)

    def append(self, val):
        if self.capacity == self.n:
            self.resize(2 * self.capacity)
        self.data[self.n] = val
        self.n += 1

    def resize(self, new_val):
        new_array = make_array(new_val)
        for i in range(self.n):
            new_array[i] = self.data[i]
        self.data = new_array
        self.capacity = new_val

    def extend(self, other):
        for i in other:
            self.append(i)

    def insert(self, index, val):
        if index == len(self):
            self.append(val)
        elif index > self.n or index < -self.n:
            raise IndexError("MyList index is out of range.")
        else:
            for i in range(len(self) - 1, index - 1, -1):
                if i == len(self) - 1:
                    self.append(self[i])
                else:
                    self[i + 1] = self[i]
            self[index] = val

    def pop(self):
        if len(self) == 0:
            raise IndexError("MyList object is empty.")
        else:
            last = self[self.n - 1]
            self.data[self.n - 1] = None
            self.n -= 1
            if self.n * 4 < self.capacity:
                self.capacity //= 2
            return last