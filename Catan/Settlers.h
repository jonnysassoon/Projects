//
//  Settlers.h
//  Catan
//
//  Created by Jonathan Sassoon on 4/4/18.
//  Copyright © 2018 Jonathan Sassoon. All rights reserved.
//

#ifndef Settlers_h
#define Settlers_h

#include "ConcentricGraph.h"
#include "Board.h"
#include "Player.h"
#include "Pieces.h"
#include <iostream>
#include <set>
#include <vector>
#include <stack>

namespace Catan {
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
            std::string roll() {
                srand(time(nullptr));
                int randomNum = rand();
                int theRoll = (randomNum % 6) + 1;
                if (theRoll <= 3) color = "black";
                else if (theRoll == 4) color = "blue";
                else if (theRoll == 5) color = "yellow";
                else color = "green";
                return color;
            }
            std::string color;
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
        Settlers(const std::vector<std::string>& playerNames);
        ~Settlers();
        void showBoard() const;
        void state() const;
        void play(); // game engine
    private:
        std::vector<Player*> players;
        std::stack<Progress*> scienceCards;
        std::stack<Progress*> politicsCards;
        std::stack<Progress*> tradeCards;
        Board gameBoard;
        Dice dice;
        BarbarianShip barbarians;
        bool robberInPlay;
        int defense;
        std::map<Player*, std::set<int>> canRaze;
        std::vector<Player*> strengthLeaders;
        std::set<Player*> hasAqueduct; // special science ability
        std::pair<Player*, int> longestRoad;
        std::map<int, Player*> activeKnightLocs;
        std::set<int> actThisTurnLocs; // keep track of all the knights that were activated this turn. When the turn is over, reset them
        std::map<std::string, std::pair<Player*, int>> metropoli; // map the type to a tuple of the Player who has it along with the location of that city
        Player* leader;
        int turnTicker;
        
        bool buildSettlement(int loc, Player* player, bool firstTurn = false);
        bool buildRoad(int loc, Player* player, bool firstTurn = false);
        bool buildCity(int loc, Player* player, bool firstTurn = false);
        bool buildWall(int loc, Player* player);
        bool buildKnight(int loc, char level, Player* player);
        bool upgradeKnight(int loc, char level, Player* player);
        bool activateKnight(int loc, Player* player);
        bool deactivateKnight(int loc, Player* player, bool deactiveAll = false); // fixing the list for every deactivation when we are deactivating everything is constly and unnecessary
        bool buildMetropolis(std::string type, int loc, Player* player);
        void moveRobber();
        
        void resolveEvent(const std::string& color, int red);
        void resolveSeven();
        void resolveRoll(int roll);
        void raze(Player* player);
        
        void buildActions();
        void turn();
    };
}


#endif /* Settlers_h */
