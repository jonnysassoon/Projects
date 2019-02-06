//
//  Player.cpp
//  Catan
//
//  Created by Jonathan Sassoon on 2/1/18.
//  Copyright © 2018 Jonathan Sassoon. All rights reserved.
//

#include "Player.h"
#include "Pieces.h"
#include <string>
#include <iostream>
using namespace std;


// How do players interract with the board?
    // In the game engine, there will be a prompt for their action
    // The player types the action command, say "build settlement"
    // The engine will ask where they want to place it, they enter the node id
    // The Player::buildSettlement() method is called
    // If it returns true (and therefore updating all of the necessary data
    // in the players member variables), the game Engine creates a settlement
    // and places it on the board.
    //
    // In other words, these methods should just check if the desired action
    // is within the player's abilities, and update the player's data accordingly
    // the affect these actions have on the board or the overall state of the game
    // will be implemented in the main.cpp file in the Settlers class
namespace Catan {
    
    Player::Player(const string& name) : vp(0), cap(7), handSize(0), roadLength(0), hasLongestRoad(false),
        knightStrength(0),
        name(name), collected(false), metropoli(0),
        // TODO: how many pieces for each category does the player start with?
        pieces({{"settlement", 5}, {"city", 4}, {"road", 15}, {"knight1", 2}, {"knight2", 2}, {"knight1", 2}, {"wall", 3}}),
        resources({{"sheep", 0}, {"brick", 0}, {"wheat", 0}, {"wood", 0}, {"ore", 0}, {"coin", 0}, {"paper", 0}, {"silk", 0}}),
        citImprov({{"trade", 0}, {"politics", 0}, {"science", 0}}) {}

    void Player::display() const {
        cout << "Player: " << name << "\n\tVictory points: " << vp << "\n\tResource hand size: " << handSize << endl;
        showResources();
        cout << "\n\tPlayer's longest road is " << roadLength << " roads long and they " ;
        if (hasLongestRoad) cout << "do ";
        else cout << "do not ";
        cout << "have longest road\n\tKnight strength: " << knightStrength << "\n\tPlayer has " << metropoli << " metropoli: "   << "\n\tCity improvements:\n\t\ttrade:" << citImprov.at("trade") << "\n\t\tpolitics:" << citImprov.at("politics") <<"\n\t\tscience:" << citImprov.at("science") << "\n\tPlayer has " << progCards.size() << " progress cards" << endl;
    }
    
    string Player::getName() const { return name; }
    
    int Player::getPoints() const { return vp; }
    
    int Player::getCapacity() const { return cap; }
    
    int Player::getHandSize() const { return handSize; }
    
    int Player::getKnightStrength() const { return knightStrength; }
    
    int Player::getCitImprovements(const string& type) const { return citImprov.at(type); }
    
    int Player::getRoadLength() const { return roadLength; }
    
    void Player::setRoadLength(int len) { roadLength = len; }
    
    bool Player::hasAbility(const string& type) const { return citImprov.at(type) >= 3; }
    
    bool Player::hasResource(const string& resource) const { return resources.at(resource) > 0; }
    
    bool Player::collectedThisRound() const { return collected; }
    
    void Player::discard(const string& resource) {
        vector<string> toSpend{resource};
        spend(toSpend);
    }
    
    void Player::resetCollected() { collected = false; }
    
    bool Player::buildSettlement(bool firstTurn) {
        if (!firstTurn) { // if it is the first turn, it's free
            vector<string> necessary = {"sheep", "brick", "wheat", "wood"};
            for (const string& aRes : necessary) {
                if (resources[aRes] < 1) {
                    cout << name << " is missing " << aRes << " and can't build a settlement.\n";
                    return false;
                }
            }
            if (pieces["settlement"] < 1) {
                cout << name << " can't build a settlement because they do not have another settlement piece.\n";
                return false;
            }
            spend(necessary);
        }
        pieces["settlement"]--; // Note: if a player built 5 settlements, 1 city and gets razed, he loses the city but still is able to build a settlement. In such a case, pieces["settlements"] will be -1. This shouldn't cause gameplay error because he still won't be able to build another settlement, and once he upgrades some settlement to a city, pieces["settlement"] will accurately reflect that he has 0.
        vp++;
        return true;
    }
    
    bool Player::buildRoad(bool firstTurn) {
        if (!firstTurn) {
            vector<string> necessary = {"brick", "wood"};
            for (const string& aRes : necessary) {
                if (resources[aRes] < 1) {
                    cout << name << " is missing " << aRes << " and can't build a road.\n";
                    return false;
                }
            }
            if (pieces["road"] < 1){
                cout << name << " can't build a road because they do not have another road piece.\n";
                return false;
            }
            spend(necessary);
        }
        pieces["road"]--;
        return true;
    }
    
    bool Player::buildCity(bool firstTurn) {
        if (!firstTurn) {
            vector<string> necessary = {"wheat", "wheat", "ore", "ore", "ore"};
            if (resources["ore"] < 3 || resources["wheat"] < 2) {
                cout << name << " does not have enough";
                if (resources["ore"] < 3 && resources["wheat"] < 2) cout << " wheat and ore";
                else if (resources["ore"] < 3) cout << " ore";
                else if (resources["wheat"] < 2) cout << " wheat";
                cout << " and can't build a city.\n";
                return false;
            }
            if (pieces["city"] < 1) {
                cout << name << " can't build a city because they do not have another city piece.\n";
                return false;
            }
            spend(necessary);
            pieces["settlement"]++;
            vp--; // take away a settlement
        }
        vp += 2; // add a city
        pieces["city"]--;
        return true;
    }
    
    bool Player::destroyCity() {
        if (pieces["city"] == 4) return false; // I don't think this is ever really necessary
        vp -= 2; // we deduct 2 because he will immediately build a free settlement
        pieces["city"]++;
        return true;
    }
    
    bool Player::buildCitImprove(const string& type) {
        int pastFlips = Player::getCitImprovements(type);
        if (pieces["city"] == 4 || // if he hasn't yet built a city...
            (4 - pieces["city"] == metropoli // ...or he has a metropolis on all the cities he did built and yet...
             && pastFlips == 3) // ...he wants to do his fourth flip (which would give him another metropolis)
            ) return false;
        if (pastFlips == 5) return false; // maxed out on flips
        string commodity;
        if (type == "science") commodity = "paper";
        else if (type == "politics") commodity = "coin";
        else if (type == "trade") commodity = "silk";
        if (resources[commodity] < pastFlips+1) return false; // insufficient
        vector<string> necessary;
        for (int i = 0; i < pastFlips+1; i++) necessary.push_back(commodity);
        spend(necessary);
        citImprov[type]++;
        return true;
    }
    
    bool Player::buildWall() {
        vector<string> necessary{"brick", "brick"};
        if (resources["brick"] < 2) return false;
        if (pieces["wall"] < 1) return false;
        spend(necessary);
        pieces["wall"]--;
        cap += 2;
        return true;
    }
    
    bool Player::buildKnight(char level) {
        if (level != '1' && level != '2' && level != '3') return false;
        int levelInt = level % 48; // turns '3' into 3
        vector<string> necessary;
        for (int i = 0; i < levelInt; i++) { // for every level you want to build, add a sheep + ore
            necessary.push_back("sheep");
            necessary.push_back("ore");
        }
        if (resources["ore"] < levelInt || resources["sheep"] < levelInt) return false;
        if (level == '3' and !hasAbility("politics")) return false;
        string knight("knight");
        if (pieces[knight + level] < 1) return false;
        spend(necessary);
        pieces[knight + level]--;
        return true;
    }
    
    bool Player::upgradeKnight(char currLevel, char toLevel) {
        if (toLevel != '2' && toLevel != '3') return false;
        int toLevelInt = toLevel % 48; // turns '3' into 3
        int currLevelInt = currLevel % 48;
        int difference = toLevelInt - currLevelInt;
        if (difference < 1 || difference > 2) return false; // only valid upgrade if 1 <= difference <= 2
        vector<string> necessary;
        for (int i = 0; i < difference; i++) { // for every level difference you want to upgrade, add a sheep + ore
            necessary.push_back("sheep");
            necessary.push_back("ore");
        }
        if (resources["ore"] < difference || resources["sheep"] < difference) return false;
        string knight("knight");
        if (pieces[knight + toLevel] < 1) return false; // don't have anymore of those pieces
        if (toLevel == '3' and !hasAbility("politics")) return false;
        spend(necessary);
        pieces[knight+toLevel]--;
        pieces[knight+currLevel]++;
        return true;
    }
    
    bool Player::activateKnight(int strength) {
        vector<string> necessary{"wheat"};
        if (resources["wheat"] < 1) return false;
        spend(necessary);
        knightStrength += strength;
        return true;
    }

    bool Player::deactivateKnight(int strength) {
        knightStrength -= strength;
        return true;
    }
    
    bool Player::buildMetropolis() {
        vp += 2;
        metropoli++;
        return true;
    }
    
    bool Player::removeMetropolis() {
        vp -= 2;
        metropoli--;
        return true;
    }
    
    void Player::rob(Player* other) {
        vector<string> othersHand;
        // add resource names the number of times his resource library says he has that resource
        for (auto keyIter = other->resources.begin(); keyIter != other->resources.end(); keyIter++) {
            for (int i = 0; i < keyIter->second; i++) othersHand.push_back(keyIter->first); // if he doesn't have any, this loop won't begin
        }
        int randomNum = rand();
        string pick = othersHand[randomNum % othersHand.size()];
        other->resources[pick]--;
        other->handSize--;
        resources[pick]++;
        handSize++;
    }

    void Player::collectResource(const string& resource) {
        resources[resource]++;
        handSize++;
        collected = true;
        cout << name << " collects: " << resource << endl;
    }
    
    void Player::defendCatan() { vp++; }
    
    void Player::giveLongestRoad() {
        hasLongestRoad = true;
        vp += 2;
    }
    
    void Player::removeLongestRoad() {
        hasLongestRoad = false;
        vp -= 2;
    }
    
    void Player::spend(const vector<string>& spending){
        for (const string& resource : spending) {
            resources[resource]--;
            handSize--;
        }
    }
    
    bool Player::moveDisplaced(Knight* theKnight) {
        int loc;
        cout << "Where would you like to move it to?\n";
        cin >> loc;
        if (loc == -1) { // code for "i can't move this anywhere"
            if (theKnight->activated) knightStrength -= theKnight->strength;
            // add a knight of its strength to the players pieces library
            delete theKnight;
            return true;
        }
//        while (/*loc position is invalid*/) {
            cout << "Invalid location\n";
            int iloc;
            cout << "Where would you like to move it to?\n";
            cin >> iloc;
//        }
        // place the knight at this location
        return true;
    }
    
    bool Player::moveKnight(Knight* theKnight) {
        int loc;
        cout << "Where would you like to move it to?\n";
        cin >> loc;
        // if (/*loc is not valid*/) return false;
        // if (loc has other knight)
        // if (other knight strength >= this knight) return false
//            (otherknight->owner).moveDisplaced(otherknight);
        // place the knight there
        return true;
    }
    
    void Player::showResources() const {
        for (auto iter = resources.begin(); iter != resources.end(); iter++) {
            if (iter->second != 0) cout << '\t' << iter->first << ':' << iter->second << endl;
        }
    }
}
