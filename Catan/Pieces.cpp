//
//  Pieces.cpp
//  Catan
//
//  Created by Jonathan Sassoon on 2/2/18.
//  Copyright © 2018 Jonathan Sassoon. All rights reserved.
//

#include "Pieces.h"
#include "Player.h"
using namespace std;

namespace Catan {
    
    Settlement::Settlement(Player* owner) : owner(owner) {}
    
    Metropolis::Metropolis(const string& color) : color(color) {}
    
    City::City(Player* owner) : owner(owner), wall(false), metropolis(nullptr) {}
    
    Knight::Knight(int strength, Player* owner) : strength(strength), activated(false), activeThisRound(false), owner(owner) {}
    
    Merchant::Merchant() : owner(nullptr) {}
    
    ostream& operator<<(ostream& os, const Knight& rhs) {
        if (rhs.activated) os << "Active";
        else os << "Non active";
        os << " knight is owned by " << rhs.owner->getName() << " and is of strength " << rhs.strength;
        return os;
    }
}
