//  Author: Jonny Sassoon
//  Program: Catan Player
//
//  Header file for the Catan player
//
#ifndef Player_h
#define Player_h
#include "Card.h"
#include <string>
#include <vector>
#include <map>
#include <set>

namespace Catan {
    class Knight;
    class Progress;
    
    // TODO: double check - given change of implementation approach, all loc arguments may be uneccessary
    class Player {
    public:
        Player(const std::string& name);
        void display() const;
        std::string getName() const;
        int getCitImprovements(const std::string& type); // why can't I make this const?
        bool buildSettlement();
        bool buildRoad();
        bool buildCity();
        bool buildknight(); // this only builds level 1 knights. if a player wants to build a level two knight in one move, the program will execute a buildKnight() followed by an upgradeKnight
        bool upgradeKnight(char level);
        bool activateKnight();
        bool deactivateKnight();
        bool moveRobber();
        bool buildCitImprove(const std::string& type);
        bool buildWall();
        void defendCatan();
    private:
        int vp; // victory points
        int Cap; // how many resources/commodities
        int handSize; // number of resources in hand currently
        int roadLength;
        int knightStrength;
        std::string name;
        std::map<std::string, int> pieces; // map piece names to the amount player has
        std::map<std::string, int> resources; // map resource/commodity names to the amount player has
        std::vector<Progress*> progCards;
        std::map<std::string, int> citImprov; // map "politics"/"science"/"trade" to the amount of flips
        // ports?
        void spend(const std::vector<std::string>& spending);
        bool moveKnight(Knight* theKnight);
        bool moveDisplaced(Knight* theKnight);
        bool rob(Player* other);
    };
}

#endif /* Player_h */
