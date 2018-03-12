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
        rollForFirst();
        placeInitialSettlements();
        while (leader->getPoints() != 13) {
            turn();
        }
    }
    void turn() { // maybe this should also be a private method
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
    void buildSettlement(int loc, Player* player, bool firstTurn = false) {
        Settlement* setptr = new Settlement(player);
        player->buildSettlement(firstTurn);
        gameBoard.placeSettlement(loc, setptr);
        cout << player->getName() << " builds a settlement on position " << loc << endl << "Where do you want to build the road?\n";
    }
    void buildRoad(int loc, Player* player, bool firstTurn = false) {
        Road* roadptr = new Road(player);
        player->buildRoad(firstTurn);
        gameBoard.placeRoad(loc, roadptr);
        cout << player->getName() << " builds a road on position " << loc << endl;
    }
    void buildCity(int loc, Player* player, bool firstTurn = false) {
        City* cityptr = new City(player);
        player->buildCity(firstTurn);
        gameBoard.placeCity(loc, cityptr, firstTurn);
        cout << player->getName() << " builds a city on position " << loc << endl;
    }
    void rollForFirst() {
        map<Player*, int> firstRoll;
        pair<Player*, int> goesFirst{nullptr, -1};
        int turnTickInit = 0;
        for (Player* player : players) { // If tie, the guy who rolled first wins... (I'm lazy)
            int roll = dice.roll();
            firstRoll[player] = roll;
            if (roll > goesFirst.second) {
                goesFirst.first = player;
                goesFirst.second = roll;
                turnTicker = turnTickInit;
            }
            turnTickInit++;
        }
    }
    void placeInitialSettlements() {
        for (int i = turnTicker; i < turnTicker + players.size(); i++) {
            Player* player = players[i % players.size()];
            cout << player->getName() << "'s turn to build a settlement\nWhere do you want to build it?\n";
            int setLoc;
            cin >> setLoc;
            while (!gameBoard.isValidSetLoc(setLoc, player)) {
                cout << "That position is invalid, please choose again\nWhere do you want to build it?\n";
                int setLoc;
                cin >> setLoc;
            }
            buildSettlement(setLoc, player, true);
            int roadLoc;
            cin >> roadLoc;
            while (!gameBoard.isValidFirstRoadLoc(roadLoc, setLoc)) {
                cout << "That position is invalid, please choos again\nWhere do you want to build the road?\n";
                int roadLoc;
                cin >> roadLoc;
            }
            buildRoad(roadLoc, player, true);
        }
        for (int i = 1; i < players.size()+1; i++) {
            int playerInd = turnTicker-i;
            if (playerInd < 0) playerInd += players.size(); // wrap around
            Player* player = players[playerInd];
            cout << player->getName() << "'s turn to build a settlement\nWhere do you want to build it?\n";
            int citLoc;
            cin >> citLoc;
            while (!gameBoard.isValidSetLoc(citLoc, player)) { // on the first turn, a valid city is a valid settlement
                cout << "That position is invalid, please choose again\nWhere do you want to build it?\n";
                int setLoc;
                cin >> setLoc;
            }
            buildCity(citLoc, player, true); // includes giving player corresponding resources
            int roadLoc;
            cin >> roadLoc;
            while (!gameBoard.isValidFirstRoadLoc(roadLoc, citLoc)) {
                cout << "That position is invalid, please choos again\nWhere do you want to build the road?\n";
                int roadLoc;
                cin >> roadLoc;
            }
            buildRoad(roadLoc, player, true);
        }
        leader = players[turnTicker]; // they're all tied, just make the first guy the leader
    }
    
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
