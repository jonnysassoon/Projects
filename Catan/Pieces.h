//  Author: Jonny Sassoon
//  Program: Catan Pieces
//
//  Header file for all Catan pieces that can be placed on the board
//
//

#ifndef Pieces_h
#define Pieces_h
#include <string>
#include <iostream>

namespace Catan {
    class Player;
    
    // TODO: consider the const-ness of these pointers
    struct Settlement {
        Settlement(Player* owner);
        Player* owner;
    };
    
    struct Metropolis {
        Metropolis(const std::string& color);
        std::string color;
    };
    
    struct City {
        City(Player* owner);
        Player* owner;
        bool wall;
        Metropolis* metropolis;
    };
    
    struct Knight {
        Knight(int strength, Player* owner);
        int strength;
        bool activated;
        bool activeThisRound;
        Player* owner;
    };
    
    struct Robber {
        int previousLoc;
    };
    
    struct Merchant {
        Merchant();
        Player* owner;
        std::string resource;
     };
    
    std::ostream& operator<<(std::ostream& os, const Knight& rhs);
}

#endif /* Pieces_h */
