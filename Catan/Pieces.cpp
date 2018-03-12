//
//  Pieces.cpp
//  Catan
//
//  Created by Jonathan Sassoon on 2/2/18.
//  Copyright © 2018 Jonathan Sassoon. All rights reserved.
//

#include "Pieces.h"
using namespace std;

namespace Catan {
    
    Settlement::Settlement(Player* owner) : owner(owner) {}

    Road::Road(Player* owner) : owner(owner) {}
    
    Metropolis::Metropolis(const string& color) : color(color) {}
    
    CityWall::CityWall(Player* owner) : owner(owner) {}

    City::City(Player* owner) : owner(owner), wall(nullptr), metropolis(nullptr) {}
    
    Knight::Knight(int strength, Player* owner) : strength(strength), activated(false), activeThisRound(false), owner(owner) {}
    
    Merchant::Merchant() : owner(nullptr) {}
}
