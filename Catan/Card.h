//  Author: Jonny Sassoon
//  Program: Catan Cards
//
//  Header file for all cards in the game

#ifndef Card_h
#define Card_h
#include <string>

namespace Catan {
    // RESOURCE/COMMODITY CARDS LIKELY UNNECESSARY
    
    class Resource {
        Resource(const std::string& type);
        std::string type;
    };
    
    class Progress {
        
    };
}

#endif /* Card_h */
