import random
from UnsortedArrayMap import UnsortedArrayMap

class ChainingHashTableMap:
    def __init__(self, N=64, p=6460101079):
        self.N = N
        self.table = [None] * self.N
        self.data_order = [] # allows for a FIFO iteration
        self.spot_in_line = 0
        self.n = 0
        self.p = p
        self.a = random.randint(1, self.p - 1)
        self.b = random.randint(0, self.p - 1)

    def hash_function(self, k):
        int_code_of_k = hash(k)
        ind = ((self.a*int_code_of_k + self.b) % self.p) % self.N
        return ind

    def __len__(self):
        return self.n

    def is_empty(self):
        return (len(self) == 0)

    def __getitem__(self, key):
        j = self.hash_function(key)
        curr_bucket = self.table[j] #list
        if (curr_bucket is None):
            raise KeyError(str(key) + " not found")
        for data_tup in curr_bucket: #effectively replacing __getitem__ method of UAM
            if data_tup[0].key == key:
                return data_tup[0].value
        raise KeyError(str(key) + " not found")

    def __setitem__(self, key, value):
        j = self.hash_function(key)
        if(self.table[j] is None):
            self.table[j] = []
        for data_tup in self.table[j]: #effectively replacing __setitem__ method of UAM
            if data_tup[0].key == key:
                data_tup[0].value = value
                return
        item = UnsortedArrayMap.Item(key, value)
        item_with_spot = (item, self.spot_in_line)
        self.table[j].append(item_with_spot)
        self.data_order.append(key)
        self.n += 1
        self.spot_in_line += 1
        if (self.n > self.N):
            self.rehash(2 * self.N)

    def __delitem__(self, key):
        j = self.hash_function(key)
        curr_bucket = self.table[j]
        if(curr_bucket is None):
            raise KeyError(str(key) + " not found")
        for i in range(len(curr_bucket)): #effectively replacing the del method of UAM
            data_tup = curr_bucket[i]
            if data_tup[0].key == key:
                self.data_order[data_tup[1]] = None #retain ability to append while corresponding to index of spot_in_line
                curr_bucket.pop(i)
                if len(curr_bucket) == 0:
                    self.table[j] = None
                self.n -= 1
                if (self.n < self.N // 4):
                    self.rehash(self.N // 2)
                return
        raise KeyError(str(key) + " not found")

    def __iter__(self):
        for key_reference in self.data_order:
            if key_reference != None:
                yield key_reference

    def rehash(self, new_size):
        old_data = []
        for key in self:
            value = self[key]
            old_data.append((key, value))
        self.table = [None] * new_size
        self.data_order = []
        self.spot_in_line = 0
        self.N = new_size
        self.n = 0
        for (key, value) in old_data:
            self[key] = value