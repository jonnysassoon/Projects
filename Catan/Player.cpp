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


// TODO: I need "cancel" options for all of these methods
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
    
    Player::Player(const string& name) : vp(0), Cap(7), handSize(0), roadLength(0),
        knightStrength(0),
        name(name),
        // TODO: how many pieces for each category does the player start with?
        pieces({{"settlement", 5}, {"city", 4}, {"road", 15}, {"knight1", 2}, {"knight2", 2}, {"knight1", 2}, {"wall", 3}}),
        resources({{"sheep", 0}, {"brick", 0}, {"wheat", 0}, {"wood", 0}, {"ore", 0}, {"coin", 0}, {"paper", 0}, {"silk", 0}}),
        citImprov({{"trade", 0}, {"politics", 0}, {"science", 0}}) {}

    void Player::display() const {
        cout << "Player: " << name << "\n\tVictory points: " << vp << "\n\tHand size: " << handSize << "\n\tRoad length: " << roadLength << "\n\tKnight strength: " << knightStrength << endl;
    }
    
    string Player::getName() const { return name; }
    
    int Player::getCitImprovements(const string& type) { // returns number of flips
        return citImprov[type];
    }
    
    bool Player::buildSettlement() {
//       this should be in main.cpp when the command is entered:
                //  if (/*loc is not valid*/) return false;
        vector<string> necessary = {"sheep", "brick", "wheat", "wood"};
        for (const string& aRes : necessary) {
            if (resources[aRes] < 1) return false;
        }
        if (pieces["settlement"] < 1) return false;
        spend(necessary);
        pieces["settlement"]--;
        vp++;
        return true;
    }
    
    bool Player::buildRoad() {
//        if (/*loc is not valid*/) return false;
        vector<string> necessary = {"brick", "wood"};
        for (const string& aRes : necessary) {
            if (resources[aRes] < 1) return false;
        }
        if (pieces["road"] < 1) return false;
        spend(necessary);
        pieces["road"]--;
        return true;
    }
    
    bool Player::buildCity() {
//        if (/*loc is not valid*/) return false;
        vector<string> necessary = {"wheat", "wheat", "ore", "ore", "ore"};
        if (resources["ore"] < 3 || resources["wheat"] < 2) return false;
        if (pieces["city"] < 1) return false;
        spend(necessary);
        pieces["city"]--;
        pieces["settlement"]++;
        vp++;
        return true;
    }
    
    bool Player::buildknight() {
//        if (/*loc is not valid*/) return false;
        vector<string> necessary = {"ore", "sheep"};
        if (resources["ore"] < 1 || resources["sheep"] < 1) return false;
        if (pieces["knight1"] < 1) return false;
        spend(necessary);
        pieces["knight1"]--;
        return true;
    }
    
    bool Player::upgradeKnight(char level) {
//        if (/*loc is not valid*/) return false;
        if (level != '1' && level != '2' && level != '3') return false;
        vector<string> necessary = {"ore", "sheep"};
        if (resources["ore"] < 1 || resources["sheep"] < 1) return false;
        string knight("knight");
        if (pieces[knight + level] < 1) return false;
        if (level == '3' and citImprov["politics"] < 3) return false;
        // if level - currKnight strength != 1 return false;
        spend(necessary);
        // put the new knight on the board
        pieces[knight+level]--;
        // pieces["knight-level"]++;
        return true;
    }
    
    bool Player::activateKnight() {
        // if (/*loc is not valid*/) return false;
        Knight* theKnight = nullptr; // TODO: this shouldn't be how it's implemented
        // if (already activated) return false;
        vector<string> necessary{"wheat"};
        if (resources["wheat"] < 1) return false;
        spend(necessary);
        theKnight->activated = true;
        theKnight->activeThisRound = true; // TODO: figure out when to set this to false
        knightStrength += theKnight->strength;
        return true;
    }
    
    bool Player::deactivateKnight() {
        // if (/*loc is not valid*/) return false;
        Knight* theKnight = nullptr; /* = getTheKnightAtThatLocation();*/
         if (!theKnight->activated || theKnight->activeThisRound) return false;
        string action;
        cout << "What would you like to do with the knight (move/dispell)\n";
        cin >> action;
        if (action == "cancel") return false;
        theKnight->activated = false;
        if (action == "move") {
            moveKnight(theKnight);
        }
        if (action == "dispell") {
            moveRobber();
        }
        knightStrength -= theKnight->strength;
        return true;
    }
    
    bool Player::moveRobber() {
        int newLoc;
        cout << "Where would you like to move the robber to?\n";
        cin >>newLoc;
        if (newLoc == -1) return false; // code for "cancel"
        set<Player*> playersToRob;
        if (1 <= newLoc && newLoc <= 54 /*&& newLoc != previous robber location*/) {
            // go to the tiles map, set that tile to blocked = true
            // for node in that tiles adjacencies
                // if that node has a settlement or a city whose owner is not you
                    // playersToRob.add(that Player pointer)
        }
        if (playersToRob.size() != 0) {
            cout << "Players to rob:\n";
            for (Player* playerptr : playersToRob) {
                playerptr->display();
                cout << endl;
            }
            string playerName;
            cout << "Who would you like to rob?\n";
            cin >> playerName;
            for (Player* playerptr : playersToRob) { // assumes player have unique names. Necessary to change? maybe choose by color? (assign colors to players)
                if (playerptr->name == playerName) {
                    rob(playerptr);
                    return true;
                }
            }
        }
        return true;
    }
    
    bool Player::buildCitImprove(const string& type) {
        int pastFlips = Player::getCitImprovements(type);
        if (pastFlips == 5) return false; // maxed out on flips
        string commodity;
        if (type == "science") commodity = "paper";
        else if (type == "politics") commodity = "coin";
        else if (type == "trade") commodity = "silk";
        if (resources[commodity] < pastFlips+1) return false; //insufficient
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
        return false;
    }
    
    void Player::defendCatan() { vp++; }
    
    void Player::spend(const std::vector<std::string>& spending){
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
    
    bool Player::rob(Player* other) {
        vector<string> othersHand;
        // add resource names the number of times his resource library says he has that resource
        for (auto keyIter = other->resources.begin(); keyIter != other->resources.end(); keyIter++) {
            for (int i = 0; i < keyIter->second; i++) othersHand.push_back(keyIter->first); // if he doesn't have any, this loop won't begin
        }
        srand(time(nullptr));
        int randomNum = rand();
        string pick = othersHand[randomNum % othersHand.size()];
        other->resources[pick]--;
        other->handSize--;
        resources[pick]++;
        handSize++;
        return true;
    }
}
