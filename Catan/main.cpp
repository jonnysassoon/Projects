//
//  main.cpp
//  Catan
//
//  Created by Jonathan Sassoon on 11/27/17.
//  Copyright © 2017 Jonathan Sassoon. All rights reserved.
//

// NOTE TO SELF: should I make a limit on resources, or just allow an arbitrary
// number of resource cards to be distributed?

#include "ConcentricGraph.h"
#include "Board.h"
#include "Player.h"
#include "Pieces.h"
#include <iostream>
#include <set>
#include <vector>
using namespace std;
using namespace Catan;

// TODO: I need "cancel" options for all of these methods

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
        string roll() {
            srand(time(nullptr));
            int randomNum = rand();
            int theRoll = (randomNum % 6) + 1;
            if (theRoll <= 3) color = "black";
            else if (theRoll == 4) color = "blue";
            else if (theRoll == 5) color = "yellow";
            else color = "green";
            return color;
        }
        string color;
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
    Settlers(const vector<string>& playerNames) {
        for (const string& name : playerNames) players.push_back(new Player(name));
        defense = 0;
        leader = nullptr;
        // TODO: create the progress cards
    }
    ~Settlers() {
        for (Player* playptr : players) delete playptr;
        // TODO: clear all collections
    }
    void play() { // game engine
        rollForFirst();
        placeInitialSettlements();
        while (leader->getPoints() != 13) {
            turn();
            for (Player* player : players) player->resetCollected(); // set player->collected to false
            turnTicker++;
            turnTicker %= players.size();
        }
        cout << leader->getName() << " wins the game!\n";
    }
private:
    vector<Player*> players;
    vector<Progress*> scienceCards;
    vector<Progress*> politicsCards;
    vector<Progress*> tradeCards;
    Board gameBoard;
    Dice dice;
    BarbarianShip barbarians;
    bool robberInPlay;
    int defense;
    set<Player*> defenders; // whenever someone activates a knight, check defenders[0]->knightStrength, if > current leader, defenders = {&thatPlayer}, elif == leader, set.emplace(&thatPlayer), else nothing
    set<Player*> hasAqueduct; // special science ability
    // largest road. Player* or int?
    Player* leader;
    int turnTicker;
    
    bool buildSettlement(int loc, Player* player, bool firstTurn = false) {
        Settlement* setptr = new Settlement(player);
        if (player->buildSettlement(firstTurn)) {
            gameBoard.placeSettlement(loc, setptr);
            cout << player->getName() << " builds a settlement on position " << loc << endl;
            return true;
        }
        cout << player->getName() << " could not build a settlement.\n";
        delete setptr;
        return false;
    }
    
    bool buildRoad(int loc, Player* player, bool firstTurn = false) {
        if (player->buildRoad(firstTurn)) {
            gameBoard.placeRoad(loc, player);
            cout << player->getName() << " builds a road on position " << loc << endl;
            return true;
        }
        cout << player->getName() << " could not build a road.\n";
        return false;
    }
    
    bool buildCity(int loc, Player* player, bool firstTurn = false) {
        City* cityptr = new City(player);
        if(player->buildCity(firstTurn)) {
            gameBoard.placeCity(loc, cityptr, firstTurn);
            barbarians.strength++;
            cout << player->getName() << " builds a city on position " << loc << endl;
            return true;
        }
        cout << player->getName() << " could not build a city.\n";
        delete cityptr;
        return false;
    }
    
    bool buildWall(int loc, Player* player) {
        if (player->buildWall()) {
            gameBoard.placeWall(loc);
            cout << player->getName() << " builds a wall on position " << loc << endl;
            return true;
        }
        cout << player->getName() << " could not build a wall.\n";
        return false;
    }
    
    bool buildKnight(int loc, char level, Player* player) {
        int levelInt = level % 48; // turns '3' into 3
        Knight* knightptr = new Knight(levelInt, player);
        if (player->buildKnight(level)) {
            gameBoard.placeKnight(loc, knightptr);
            cout << player->getName() << " builds a level " << level << " knight on position " << loc << endl;
            return true;
        }
        cout << player->getName() << " could not build a level " << level << " knight.\n";
        return false;
    }
    
    bool upgradeKnight(int loc, char level, Player* player) {
        int toLevel = level % 48; // turns '3' into 3
        int currLevelInt = gameBoard.getKnightLevel(loc);
        char currLevel = currLevelInt - '0'; // turns 3 into '3'
        Knight* knightptr = new Knight(toLevel, player);
        if (player->upgradeKnight(currLevel, toLevel)) {
            gameBoard.placeKnight(loc, knightptr);
            cout << player->getName() << " upgrades from a level " << currLevel << " knight to a level " << toLevel << " knight on position " << loc << endl;
            return true;
        }
        cout << player->getName() << " could not upgrade from a level " << currLevel << " knight to a level " << toLevel << " knight.\n";
        return false;
    }
    
    bool activateKnight(int loc, Player* player) {
        int strength = gameBoard.getKnightLevel(loc);
        if (player->activateKnight(strength)) {
            gameBoard.activateKnight(loc);
            cout << player->getName() << " activated a level " << strength << " knight on position " << loc << endl;
            defense += strength;
            return true;
        }
        cout << player->getName() << " could not active a knight on position " << loc << endl;
        return false;
    }
    
    void rollForFirst() {
        map<Player*, int> firstRoll;
        pair<Player*, int> goesFirst{nullptr, -1};
        int turnTickInit = 0;
        for (Player* player : players) { // If tie, the guy who rolled first wins... (I'm lazy)
            int roll = dice.roll();
            firstRoll[player] = roll;
            if (roll > goesFirst.second) {
                goesFirst.first = player;
                goesFirst.second = roll;
                turnTicker = turnTickInit;
            }
            turnTickInit++;
        }
    }
    
    void placeInitialSettlements() {
        for (int i = turnTicker; i < turnTicker + players.size(); i++) {
            Player* player = players[i % players.size()];
            cout << player->getName() << "'s turn to build a settlement\nWhere do you want to build it?\n";
            int setLoc;
            cin >> setLoc;
            while (!gameBoard.isValidSetLoc(setLoc, player)) {
                cout << "That position is invalid, please choose again\nWhere do you want to build it?\n";
                int setLoc;
                cin >> setLoc;
            }
            buildSettlement(setLoc, player, true);
            cout << "Where do you want to build the road?\n";
            int roadLoc;
            cin >> roadLoc;
            while (!gameBoard.isValidFirstRoadLoc(roadLoc, setLoc)) {
                cout << "That position is invalid, please choos again\nWhere do you want to build the road?\n";
                int roadLoc;
                cin >> roadLoc;
            }
            buildRoad(roadLoc, player, true);
        }
        for (int i = 1; i < players.size()+1; i++) {
            int playerInd = turnTicker-i;
            if (playerInd < 0) playerInd += players.size(); // wrap around
            Player* player = players[playerInd];
            cout << player->getName() << "'s turn to build a city\nWhere do you want to build it?\n";
            int citLoc;
            cin >> citLoc;
            while (!gameBoard.isValidSetLoc(citLoc, player)) { // on the first turn, a valid city is a valid settlement
                cout << "That position is invalid, please choose again\nWhere do you want to build it?\n";
                cin >> citLoc;
            }
            buildCity(citLoc, player, true); // includes giving player corresponding resources
            cout << "Where do you want to build the road?\n";
            int roadLoc;
            cin >> roadLoc;
            while (!gameBoard.isValidFirstRoadLoc(roadLoc, citLoc)) {
                cout << "That position is invalid, please choos again\nWhere do you want to build the road?\n";
                cin >> roadLoc;
            }
            buildRoad(roadLoc, player, true);
        }
        leader = players[turnTicker]; // they're all tied, just make the first guy the leader
    }
    
    void resolveEvent(const string& color, int red) {
        if (color == "black") {
            barbarians.move();
            if (barbarians.location == 7) {
                if (barbarians.strength > defense) { // TODO: implement raise catan
                    // raise catan
                        // if robber not in play, robberInPlay = true
                } else {
                    if (defenders.size() == 1) {
                        (*defenders.begin())->defendCatan();
                    } else {
                        for (auto it = defenders.begin(); it != defenders.end(); it++){
                            // give them the option of a progress card category
                        }
                    }
                    // deactivate everyone's knights
                }
                barbarians.reset();
            }
            return;
        } else if (color == "green") { // access to 2 : 1 flip; 3 : 2; 4 : 3...
            for (int i = 0; i < players.size(); i++) {
                if (red - players[(turnTicker + i)%players.size()]->getCitImprovements("science") <= 1) {
                    // choose a progress card
                }
            }
            return;
        } else if (color == "blue") {
            for (int i = 0; i < players.size(); i++) {
                if (red - players[(turnTicker + i)%players.size()]->getCitImprovements("politics") <= 1) {
                    // choose a progress card
                }
            }
            return;
        } else if (color == "yellow") {
            for (int i = 0; i < players.size(); i++) {
                if (red - players[(turnTicker + i)%players.size()]->getCitImprovements("trade") <= 1) {
                    // choose a progress card
                }
            }
            return;
        }
    }
    
    void resolveSeven() {
        Player* thePlayer = players[turnTicker];
        for (Player* player : players) {
            if (player->getHandSize() > player->getCapacity()) {
                cout << player->getName() << " has to discard " << player->getHandSize()/2 << " cards\n";
                string resource;
                for (int i = 0; i < player->getHandSize()/2; i++) {
                    cout << "Choose a resource or commodity you'd like to discard\n";
                    cin >> resource;
                    while (!player->hasResource(resource)) {
                        cout << "You don't have that resource or commodity\nChoose a resource or commodity you'd like to discard\n";
                        cin >> resource;
                    }
                    player->discard(resource);
                }
            }
        }
        if (robberInPlay) {
            cout << thePlayer->getName() << " gets to move the robber.\nWhere do you want to place the robber?\n";
            int loc;
            cin >> loc;
            while (!gameBoard.isValidRobberLoc(loc)) {
                cout << "The robber is already on that spot\nWhere do you want to place the robber?\n";
                cin >> loc;
            }
            set<Player*> toRob = gameBoard.placeRobber(loc);
            if (toRob.size() == 0 ||
                    (toRob.size() == 1 &&
                     (*toRob.begin())->getName() == thePlayer->getName()
                 )
                ) cout << "There is no one to rob.\n";
            else { // there's at least one person who's not you
                cout << "You can choose to rob:\n";
                for (Player* player : toRob) if (player != thePlayer) cout << player->getName() << endl;
                string playerName;
                cout << "Who would you like to rob?\n";
                cin >> playerName; // assumes all have unique names
                Player* victim = nullptr;
                for (Player* player : players){
                    if (player->getName() == playerName && player != thePlayer) victim = player;
                }
                while (victim == nullptr && playerName != "stop") {
                    cout << "The name entered was invalid\nYou can choose to rob:\n";
                    for (Player* player : toRob) cout << player->getName() << endl;
                    string playerName;
                    cout << "Who would you like to rob?\n";
                    cin >> playerName; // assumes all have unique names
                    Player* victim = nullptr;
                    for (Player* player : players) if (player->getName() == playerName) victim = player;
                } if (playerName == "stop") exit(1);
                thePlayer->rob(victim);
            }
        }
    }
    
    void resolveRoll(int roll) {
        if (roll != 7){ // collection
            gameBoard.distributeResources(roll);
            for (Player* player : hasAqueduct) { // most of the time this set will be empty
                if (!player->collectedThisRound()) {
                    cout << player->getName() << " gets to choose a resource of his choice\nWhich resource would you like (brick/wood/wheat/sheep/ore)?\n";
                    string resource;
                    cin >> resource;
                    while (resource != "brick" || resource != "wood" ||
                           resource != "wheat" || resource != "sheep" || resource != "ore") {
                        cout << "That isn't a resource\nWhich resource would you like (brick/wood/wheat/sheep/ore)?\n";
                        cin >> resource;
                    }
                    player->collectResource(resource);
                }
            }
        } else resolveSeven();
    }
    
    void buildActions() {
        Player* thePlayer = players[turnTicker];
        cout << "What would you like to build?\n";
        string toBuild;
        cin >> toBuild;
        while (toBuild != "end") {
            if (toBuild == "settlement") {
                cout << "Where would you like to build the settlement?\n";
                int loc;
                cin >> loc;
                string cancel;
                while (cancel != "yes" && !gameBoard.isValidSetLoc(loc, thePlayer)) {
                    cout << "Invalid settlement location.\nWould you like to cancel?\n";
                    cin >> cancel;
                    if (cancel != "yes") {
                        cout << "Where would you like to build the settlement?\n";
                        cin >> loc;
                    }
                }
                if (cancel != "yes") buildSettlement(loc, thePlayer);
            } else if (toBuild == "city") {
                cout << "Where would you like to build the city?\n";
                int loc;
                cin >> loc;
                string cancel;
                while (cancel != "yes" && !gameBoard.isValidCityLoc(loc, thePlayer)) {
                    cout << "Invalid city location.\nWould you like to cancel?\n";
                    cin >> cancel;
                    if (cancel != "yes") {
                        cout << "Where would you like to build the city?\n";
                        cin >> loc;
                    }
                }
                if (cancel != "yes") buildCity(loc, thePlayer);
            } else if (toBuild == "road") {
                cout << "Where would you like to build the road?\n";
                int loc;
                cin >> loc;
                string cancel;
                while (cancel != "yes" && !gameBoard.isValidRoadLoc(loc, thePlayer)) {
                    cout << "Invalid road location.\nWould you like to cancel?\n";
                    cin >> cancel;
                    if (cancel != "yes") {
                        cout << "Where would you like to build the road?\n";
                        cin >> loc;
                    }
                }
                if (cancel != "yes") buildRoad(loc, thePlayer); // CHECK ROAD LENGTH!!
            } else if (toBuild == "wall") {
                cout << "Where would you like to build the wall?\n";
                int loc;
                cin >> loc;
                string cancel;
                while (cancel != "yes" && !gameBoard.isValidWallLoc(loc, thePlayer)) {
                    cout << "Invalid wall location.\nWould you like to cancel?\n";
                    cin >> cancel;
                    if (cancel != "yes") {
                        cout << "Where would you like to build the wall?\n";
                        cin >> loc;
                    }
                }
                if (cancel != "yes") buildWall(loc, thePlayer);
            }
            else if (toBuild == "knight") {
                cout << "Do you want to construct, activate, or upgrade a knight?\n";
                string option;
                cin >> option;
                if (option == "construct") {
                    cout << "Where would you like to build the knight?\n";
                    int loc;
                    cin >> loc;
                    string cancel;
                    while (cancel != "yes" && !gameBoard.isValidKnightLoc(loc, thePlayer, false)) {
                        cout << "Invalid knight location.\nWould you like to cancel?\n";
                        cin >> cancel;
                        if (cancel != "yes") {
                            cout << "Where would you like to build the knight?\n";
                            cin >> loc;
                        }
                    }
                    if (cancel != "yes") { // you want to build a knight
                        cout << "What level knight?\n";
                        char level;
                        cin >> level;
                        string different;
                        while (different != "no" && !buildKnight(loc, level, thePlayer)) { // if you can't build a specific level knight, but you want to build some level knight, it loops
                            cout << "Do you want to build a different level knight?\n";
                            cin >> different;
                            if (different != "no") { // you want to build a different level knight
                                cout << "What level knight?\n";
                                cin >> level;
                            }
                        }
                    }
                }
                else if (option == "upgrade") {
                    cout << "Where is the knight you'd like to upgrade?\n";
                    int loc;
                    cin >> loc;
                    string cancel;
                    while (cancel != "yes" && !gameBoard.isValidKnightLoc(loc, thePlayer, true)) {
                        cout << "Invalid knight location.\nWould you like to cancel?\n";
                        cin >> cancel;
                        if (cancel != "yes") {
                            cout << "Where is the knight you'd like to upgrade?\n";
                            cin >> loc;
                        }
                    }
                    if (cancel != "yes") { // you want to upgrade a knight
                        cout << "What level knight?\n";
                        char level;
                        cin >> level;
                        string different;
                        while (!upgradeKnight(loc, level, thePlayer) && different != "no") {
                            cout << "Do you want to build a different level knight?\n";
                            cin >> different;
                            if (different != "no") { // you want to build a different level knight
                                cout << "What level knight?\n";
                                cin >> level;
                            }
                        }
                    }
                }
                else if (option == "activate") {
                    cout << "Where is the knight you'd like to activate?\n";
                    int loc;
                    cin >> loc;
                    string cancel;
                    while (cancel != "yes" && !gameBoard.canActivateKnight(loc, thePlayer)) {
                        cout << "You can't activate a knight there.\nWould you like to cancel?\n";
                        cin >> cancel;
                        if (cancel != "yes") {
                            cout << "Where is the knight you'd like to activate?\n";
                            cin >> loc;
                        }
                    } if (cancel != "yes") activateKnight(loc, thePlayer);
                }
            } else if (toBuild == "improve") {
                cout << "Which type would you like to improve?\n";
                string type;
                cin >> type;
                if (type != "cancel" && thePlayer->buildCitImprove(type)) {
                    if (type == "science" && thePlayer->hasAbility("science")) hasAqueduct.emplace(thePlayer); // if he flips more than once while hasAbility is true, the set doesn't create a duplicate
                    cout << thePlayer->getName() << " builds a city improvement for " << type << endl;
                }
            }
            if (thePlayer->getPoints() > leader->getPoints()) leader = thePlayer;
            cout << "Would you like to keep building?\n";
            string answer;
            cin >> answer;
            if (answer == "no") return;
            cout << "What would you like to build?\n";
            cin >> toBuild;
        }
    }
    
    void turn() {
        Player* thePlayer = players[turnTicker];
        cout << thePlayer->getName() << "'s turn!\n";
        cout << "Would you like to roll or play an alchemist? (roll/alchemist)n";
        string decision;
        cin >> decision;
        int roll;
        if (decision == "roll") {
            roll = dice.roll();
        } else {
            int red;
            int yellow;
            cout << "Enter red die number\n";
            cin >> red;
            while (red  < 1 || red > 6) {
                cout << "Invalid number.\nEnter red die number\n";
                cin >> red;
            }
            cout << "Enter yellow die number\n";
            cin >> yellow;
            while (yellow < 1 || yellow > 6) {
                cout << "Invalid number.\nEnter yellow die number\n";
                cin >> yellow;
            }
            roll = red + yellow;
            dice.event.roll();
        }
        resolveEvent(dice.event.color, dice.red.number);
        resolveRoll(roll);
        string action;
        cout << "What would you like to do?\n";
        cin >> action;
        while (action != "end") {
            // include actions like "viewGameState"/"viewPlayerState"...etc
            if (action == "build") {
                buildActions();
            }
            cout << "What would you like to do?\n";
            cin >> action;
        }
        // NOTE: it should be the responsibility of these actions in this class to
        // check if the action is compatibile with the STATE OF THE BOARD.
        // The corresponding methods in the player class will check if the action is
        // within the players current abilities (resources, available pieces, etc)
        // actions
            // build
                // settlement
                // city
                // road
                // knight
                // wall
                // CitImprove // if it's science, check if he has the ability, if he does, add him to the set
            // upgrade
            // deactivate knight
                // note: i'll need a different method to check if it's a valid knight move
            // trade
            // play progress card
            // end turn
    }
};

int main() {
    
}
