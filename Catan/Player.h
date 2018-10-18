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

// TODO: consider as a data member of every player a table containing the build prices for all pieces. Why might this be beneficial? Because when a player plays a progress card that changes the prices of something, that card will just temporarily change the table. Note: this might change the logic in every Player::buildX method viz. how we figure out if he has the necessary amount for all resource.

namespace Catan {
    class Knight;
    class Progress;
    
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
        int getRoadLength() const;
        void setRoadLength(int len);
        bool hasAbility(const std::string& type) const; // does the player have a special ability from flips
        bool hasResource(const std::string& resource) const;
        bool collectedThisRound() const;
        void discard(const std::string& resource);
        void resetCollected();
        bool buildSettlement(bool firstTurn = false);
        bool buildRoad(bool firstTurn = false);
        bool buildCity(bool firstTurn = false);
        bool destroyCity();
        bool buildCitImprove(const std::string& type);
        bool buildWall();
        bool buildKnight(char level);
        bool upgradeKnight(char currLevel, char toLevel);
        bool activateKnight(int strength); // strength of the knight
        bool deactivateKnight(int strength);
        bool buildMetropolis();
        bool removeMetropolis();
        void rob(Player* other);
        void collectResource(const std::string& resource); // includes commodities
        void defendCatan();
        void giveLongestRoad();
        void removeLongestRoad();
    private:
        int vp; // victory points
        int cap; // how many resources/commodities can they have
        int handSize; // number of resources in hand currently
        int roadLength; // length of player's longest road
        bool hasLongestRoad;
        int knightStrength;
        bool collected;
        int metropoli;
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
