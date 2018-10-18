//
//  main.cpp
//  Catan
//
//  Created by Jonathan Sassoon on 11/27/17.
//  Copyright © 2017 Jonathan Sassoon. All rights reserved.
//

// NOTE TO SELF: should I make a limit on resources, or just allow an arbitrary
// number of resource cards to be distributed?

#include "Settlers.h"
using namespace std;
using namespace Catan;

int main() {
    vector<string> names{"Alice", "Bob", "Charlie", "Dan"};
    Settlers game(names);
    vector<char> info = {'a'};
    game.state(info);
}
