"""
Author: Jonny Sassoon
Program: Royal Flush
This program simulates rounds of Texas HoldEm and continues to deal until
a player lands a Royal Flush
"""
from DataStructuresAndAlgorithms.DataStructures.ArrayStack import ArrayStack
import random

class Card:
    def __init__(self, suit, val):
        self.suit = suit
        self.val = val

    def __repr__(self):
        faces = {11:"J", 12:"Q", 13:"K", 14:"A"}
        face = self.val
        if face > 10:
             face = faces[face]
        return '(' + str(face) + ', ' + self.suit + ')'

class Deck:
    def __init__(self):
        self.cards = ArrayStack()
        suits = ["S", "H", "C", "D"]
        for suit in suits:
            for i in range(2, 15):
                card = Card(suit, i)
                self.cards.push(card)

    def shuffle(self):
        random.shuffle(self.cards.data)
        
    def showDeck(self):
        return self.cards.data
    
    def dealOne(self):
        return self.cards.pop()

def holdEm(numHands):
    deck = Deck()
    deck.shuffle()
    hands = [[] for i in range(numHands)]
    
    for i in range(2*numHands):
        hands[i%numHands].append(deck.dealOne())
    
    community = []
    deck.dealOne() #First burn 
    for i in range(3): #Flop
        community.append(deck.dealOne())
    deck.dealOne() #Second burn
    community.append(deck.dealOne()) #Turn
    deck.dealOne() #Final burn
    community.append(deck.dealOne()) #River
    
    return (hands, community)
        
def isRoyalFlush(hand, community):
    card1 = hand[0]
    card2 = hand[1]
    royal = [0]*5
    
    if card1.suit != card2.suit:
        return False
        
    suit = card1.suit
        
    if card1.val < 10 or card2.val < 10:
        return False
        
    royal[card1.val%5] = card1.val
    royal[card2.val%5] = card2.val
    
    for card in community:
        if card.suit == suit:
            if card.val >= 10:
                royal[card.val%5] = card.val
    if sum(royal) == 60:
        print(hand, community)
        return True
    
def findRoyal(numOfHands):
    game = holdEm(numOfHands)
    handsList = game[0]
    community = game[1]
    found = False
    
    handNum = 0
    numOfGames = 1
    while not found:
        found = isRoyalFlush(handsList[handNum], community)
        handNum += 1
        if handNum == numOfHands:
            game = holdEm(numOfHands)
            numOfGames += 1
            handsList = game[0]
            community = game[1]
            handNum = 0
            
    print("Full hands list:", handsList)
    return numOfGames

def main():
    statistics = []
    
    for i in range(10):
        print("Royal flush #" + str(i+1) + ":")
        statistics.append(findRoyal(10))
        
    print(sum(statistics)/10)
    
main()
