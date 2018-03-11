//
//  main.cpp
//  Catan
//
//  Created by Jonathan Sassoon on 11/27/17.
//  Copyright © 2017 Jonathan Sassoon. All rights reserved.
//

// NOTE TO SELF: should I make a limit on resources, or just allow an arbitrary
// number of resource cards to be distributed?

#include "ConcentricGraph.h"
#include "Board.h"
#include "Player.h"
#include "Pieces.h"
#include <iostream>
#include <set>
#include <vector>
using namespace std;
using namespace Catan;

class Settlers {
    struct Die {
        int roll() {
            srand(time(nullptr));
            int randomNum = rand();
            number = (randomNum % 6) + 1;
            return number;
        }
        int number = -1;
    };
    struct EventDie : public Die {
        string roll() {
            srand(time(nullptr));
            int randomNum = rand();
            int theRoll = (randomNum % 6) + 1;
            if (theRoll <= 3) color = "black";
            else if (theRoll == 4) color = "blue";
            else if (theRoll == 5) color = "yellow";
            else color = "green";
            return color;
        }
        string color;
    };
    struct Dice {
        int roll() {
            result = 0;
            result += yellow.roll();
            result += red.roll();
            event.roll();
            return result;
        }
        int result;
        Die yellow;
        Die red;
        EventDie event;
    };
    struct BarbarianShip {
        void move() { location++; }
        void reset() { location = 0; }
        int location = 0;
        int strength = 0;
    };
public:
    Settlers(const vector<string>& playerNames) {
        for (const string& name : playerNames) players.push_back(new Player(name));
    }
    ~Settlers() {
        for (Player* playptr : players) delete playptr;
    }
    void play() { // game engine
//        while leader->vp != 13 { keep playing }
    }
    void turn() {
        Player* player = players[turnTicker];
        cout << player->getName() << "'s turn!\n";
        // prompt player to roll or play a progress card
        int roll = dice.roll();
        resolveEvent(dice.event.color, dice.red.number);
        // option of choosing a resource of choice if player has third science improvement
        // collect resources
        
        // NOTE: it should be the responsibility of these actions in this class to
        // check if the action is compatibile with the STATE OF THE BOARD.
        // The corresponding methods in the player class will check if the action is
        // within the players current abilities (resources, available pieces, etc)
        // actions
            // build
                // settlement
                // city
                // road
                // knight
                    // upgrade
                    // deactivate
                // wall
                // CitImprove
            // deactivate knight
            // trade
            // play progress card
        
        // end turn
        turnTicker++;
        turnTicker %= players.size();
    }
private:
    vector<Player*> players;
    Board gameBoard;
    Dice dice;
    BarbarianShip barbarians;
    int defense = 0;
    set<Player*> defenders; // whenever someone activates a knight, check defenders[0]->knightStrength, if > current leader, defenders = {&thatPlayer}, elif == leader, set.emplace(&thatPlayer), else nothing
    // largest road
    Player* leader = nullptr;
    int turnTicker;
    void resolveEvent(const string& color, int red) {
        if (color == "black") {
            barbarians.move();
            if (barbarians.location == 7) {
                if (barbarians.strength > defense) {
                    // raise catan
                } else {
                    if (defenders.size() == 1) {
                        (*defenders.begin())->defendCatan();
                    } else {
                        for (auto it = defenders.begin(); it != defenders.end(); it++){
                            // give them the option of a progress card category
                        }
                    }
                    // deactivate everyone's knights
                }
                barbarians.reset();
            }
            return;
        } else if (color == "green") { // access to 2 -> 1 flip; 3 -> 2; 4 -> 3;
            for (int i = 0; i < players.size(); i++) {
                if (red - players[turnTicker + i]->getCitImprovements("science") <= 1) {
                    // choose a progress card
                }
            }
            return;
        } else if (color == "blue") {
            for (int i = 0; i < players.size(); i++) {
                if (red - players[turnTicker + i]->getCitImprovements("politics") <= 1) {
                    // choose a progress card
                }
            }
            return;
        } else if (color == "yellow") {
            for (int i = 0; i < players.size(); i++) {
                if (red - players[turnTicker + i]->getCitImprovements("trade") <= 1) {
                    // choose a progress card
                }
            }
            return;
        }
    }
};

int main() {
    
}
