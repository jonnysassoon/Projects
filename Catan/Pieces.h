//  Author: Jonny Sassoon
//  Program: Catan Pieces
//
//  Header file for all Catan pieces that can be placed on the board
//
//

#ifndef Pieces_h
#define Pieces_h
#include <string>

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
    
    struct CityWall {
        CityWall(Player* owner);
        Player* owner;
    };
    
    struct City {
        City(Player* owner);
        Player* owner;
        CityWall* wall;
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
}

#endif /* Pieces_h */
