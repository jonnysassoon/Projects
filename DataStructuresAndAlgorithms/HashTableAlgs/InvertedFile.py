"""
Author: Jonny Sassoon
Program: Inverted File
Implementation of an Inverted File Data Structure for indexing
"""
from DataStructures.ChainingHashTableMap import ChainingHashTableMap

class InvertedFile:
	def __init__(self, file_name):
		self.word_map = ChainingHashTableMap()
		self.infile = open(file_name, "r")
		file_st = self.infile.read()
		self.infile.close()

		split_lst = file_st.split()
		word_index = 0 #doing this so i don't need so many brackets
		for word in split_lst:
			if word[-1] == ',':
				word = word[:-1]
			if word[0].isupper():
				word = word.lower()
			if word in self.word_map:
				self.word_map[word].append(word_index)
			else:
				self.word_map[word] = [word_index]
			word_index += 1

	def indices(self, word): # gets all of the index for a certain word
		if word not in self.word_map:
			return []
		return self.word_map[word]