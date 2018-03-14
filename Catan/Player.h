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
        int getPoints() const;
        int getCapacity() const;
        int getHandSize() const;
        int getKnightStrength() const;
        int getCitImprovements(const std::string& type) const; // number of flips
        bool hasAbility(const std::string& type) const; // does the player have a special ability from flips
        bool hasResource(const std::string& resource) const;
        bool collectedThisRound() const;
        void discard(const std::string& resource);
        void resetCollected();
        bool buildSettlement(bool firstTurn = false);
        bool buildRoad(bool firstTurn = false);
        bool buildCity(bool firstTurn = false);
        bool buildCitImprove(const std::string& type);
        bool buildWall();
        bool buildKnight(char level);
        bool upgradeKnight(char currLevel, char toLevel);
        bool activateKnight(int strength);
        bool deactivateKnight();
        bool moveRobber();
        void rob(Player* other);
        void collectResource(const std::string& resource); // includes commodities
        void defendCatan();
    private:
        int vp; // victory points
        int cap; // how many resources/commodities can they have
        int handSize; // number of resources in hand currently
        int roadLength;
        int knightStrength;
        bool collected;
        std::string name;
        std::map<std::string, int> pieces; // map piece names to the amount player has
        std::map<std::string, int> resources; // map resource/commodity names to the amount player has
        std::vector<Progress*> progCards;
        std::map<std::string, int> citImprov; // map "politics"/"science"/"trade" to the amount of flips
        // ports?
        void spend(const std::vector<std::string>& spending);
        bool moveKnight(Knight* theKnight);
        bool moveDisplaced(Knight* theKnight);
    };
}

#endif /* Player_h */
