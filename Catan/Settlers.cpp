//
//  Settlers.cpp
//  Catan
//
//  Created by Jonathan Sassoon on 4/13/18.
//  Copyright © 2018 Jonathan Sassoon. All rights reserved.
//

#include "Settlers.h"
using namespace std;

namespace Catan {
    Settlers::Settlers(const vector<string>& playerNames) : defense(0), robberInPlay(false), turnTicker(0), longestRoad({nullptr, 4}), metropoli({{"trade", {nullptr, -1}}, {"politics", {nullptr, -1}}, {"science", {nullptr, -1}}}) {
        for (const string& name : playerNames) players.push_back(new Player(name));
        leader = players[0]; // just so it's not nullptr to begin with
        strengthLeaders.push_back(leader); // just to have someone in here
        // TODO: create the progress cards
    }
    Settlers::~Settlers() {
        for (Player* playptr : players) delete playptr;
        // TODO: clear all collections
    }
    
    vector<Player*> Settlers::getPlayers() const {
        return players;
    }
    
    void Settlers::showBoard() const {
        gameBoard.display();
    }
    
    void Settlers::state(const vector<char>& argv) const {
        /*
         a: all
         g: general
         p: all player info (call player.display())
         s: settlements and cities (metropoli, city walls)
         b: barbarians
         k: knights (overall strength, locations)
         */
        for (size_t i = 0; i < argv.size(); i++) {
            switch (argv[i]) {
                case 'a':
                    dispG();
                    cout << endl;
                    dispP();
                    cout << endl;
                    dispS();
                    cout << endl;
                    dispB();
                    cout << endl;
                    dispK();
                    cout << endl;
                    break;
                case 'g':
                    dispG();
                    break;
                case 'p':
                    dispP();
                    break;
                case 's':
                    dispS();
                    break;
                case 'b':
                    dispB();
                    break;
                case 'k':
                    dispK();
                    break;
            }
            cout << endl;
        }
    }
    
    void Settlers::dispG() const{ // display general info
        // (who's playing, whose turn, who's winning - how many points, longest road - length/player, robber position, others?)
        cout << "---GENERAL INFO---\nThe players are: ";
        for (Player* player : players) cout << player->getName() << ' ';
        cout << endl << leader->getName() << " is currently in the lead with " << leader->getPoints() << " points\nThe longest road is currently held by ";
        if (longestRoad.first == nullptr) cout << "no one";
        else cout << longestRoad.first->getName() << " and is of length " << longestRoad.second;
        cout << "\nThe robber is on tile number " << gameBoard.getRobberLoc() << "\nThe defender of Catan (knight strength = " << (*strengthLeaders.begin())->getKnightStrength() << ") is/are: ";
        for (auto iter = strengthLeaders.begin(); iter != strengthLeaders.end(); iter++) {
            if (iter != strengthLeaders.begin()) cout << ", ";
            cout << (*iter)->getName();
        }
        if (canRaze.size() != 0) {
            cout << "\nThe razeable cities are:";
            for (auto iter = canRaze.begin(); iter != canRaze.end(); iter++) {
                cout << "\n\tFor " << iter->first->getName() << ':';
                if (iter->second.size() != 0){
                    for (auto subIter = iter->second.begin(); subIter != iter->second.end(); subIter++) cout << "\n\t\t" << *subIter;
                } else cout << " NONE";
                cout << endl;
            }
        }
        if (hasAqueduct.size() != 0) {
            for (auto iter = hasAqueduct.begin(); iter != hasAqueduct.end(); iter++) {
                if (iter != hasAqueduct.begin()) cout << ", ";
                cout << (*iter)->getName();
            }
            cout << " has/have the special science ability";
        }
        cout << endl;
    }
    void Settlers::dispP() const{ // display player info
        cout << "---PLAYER INFO---\n";
        for (Player* player : players) player->display();
    }
    void Settlers::dispS() const{ // display settlement and city info
        cout << "---SETTLEMENT AND CITY INFO---\n";
        gameBoard.dispSetCit();
    }
    void Settlers::dispB() const{ // display barbarian info
        cout << "---BARBARIAN INFO---\n";
        cout << "The barbarians are currently " << barbarians.location << " steps into their journey.\nThey have a strength of " << barbarians.strength << endl;
    }
    void Settlers::dispK() const{ // display kight info
        cout << "---KNIGHT INFO---\n";
        cout << "The total knight strength from all players is: " << defense << "\nThe active knights on the board are at locations: ";
        if (activeKnightLocs.size() > 0) {
            for (auto iter = activeKnightLocs.begin(); iter != activeKnightLocs.end(); iter++){
                int loc = iter->first;
                int strength = gameBoard.getKnightLevel(loc);
                Player* owner = iter->second;
                cout << "\n\t" << loc << " - (owner: " << owner->getName() << "; strength: " << strength << ')';
            }
        } else cout << "NONE";
        cout << endl;
    }
    
    void Settlers::play() { // game engine
        /* Roll for first */
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
        /* Place initial settlements */
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
        
        while (leader->getPoints() < 13) {
            turn();
            for (Player* player : players) player->resetCollected(); // set player->collected to false
            turnTicker++;
            turnTicker %= players.size();
        }
        cout << leader->getName() << " wins the game!\n";
    }
    
    void Settlers::buildActions() { // should this have its own implementation file?
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
                    if (thePlayer->getCitImprovements(type) >= 4) {
                        pair<Player*, int> leaderData = metropoli[type];
                        Player* leader = leaderData.first;
                        if (leader == nullptr ||
                            (leader != nullptr && leader != thePlayer &&
                             leader->getCitImprovements(type) > thePlayer->getCitImprovements(type))
                            ) {
                            // for now, just assume the player has a city that he can put it on
                            cout << "You get to build the " << type << " metropolis! Where would you like to place it?\n";
                            int loc;
                            cin >> loc;
                            while (!gameBoard.isOpenCityLoc(loc, thePlayer)) {
                                cout << "Invalid metropolis location.\nWhere would you like to place the " << type << " metropolis?\n";
                                int loc;
                                cin >> loc;
                            }
                            buildMetropolis(type, loc, thePlayer);
                        }
                    }
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
    
    bool Settlers::buildSettlement(int loc, Player* player, bool firstTurn) {
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
    
    bool Settlers::buildRoad(int loc, Player* player, bool firstTurn) {
        if (player->buildRoad(firstTurn)) {
            int roadLength = gameBoard.placeRoad(loc, player);
            cout << player->getName() << " builds a road on position " << loc << endl;
            if (roadLength > player->getRoadLength()) {
                player->setRoadLength(roadLength);
                cout << player->getName() << " increased their road length to " << player->getRoadLength() << endl;
            }
            if (roadLength > longestRoad.second) { // is this road longer than the current longest road?
                longestRoad.second = roadLength;
                if (player != longestRoad.first) { // the person with longest road is over taken
                    cout << player->getName() << " overtook " << longestRoad.first->getName() << " for longest road\n";
                    longestRoad.first->removeLongestRoad();
                    player->giveLongestRoad();
                    longestRoad.first = player;
                }
            }
            return true;
        }
        cout << player->getName() << " could not build a road.\n";
        return false;
    }
    
    bool Settlers::buildCity(int loc, Player* player, bool firstTurn) {
        City* cityptr = new City(player);
        if(player->buildCity(firstTurn)) {
            gameBoard.placeCity(loc, cityptr, firstTurn);
            barbarians.strength++;
            canRaze[player].insert(loc);
            cout << player->getName() << " builds a city on position " << loc << endl;
            return true;
        }
        cout << player->getName() << " could not build a city.\n";
        delete cityptr;
        return false;
    }
    
    bool Settlers::buildWall(int loc, Player* player) {
        if (player->buildWall()) {
            gameBoard.placeWall(loc);
            cout << player->getName() << " builds a wall on position " << loc << endl;
            return true;
        }
        cout << player->getName() << " could not build a wall.\n";
        return false;
    }
    
    bool Settlers::buildKnight(int loc, char level, Player* player) {
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
    
    bool Settlers::upgradeKnight(int loc, char level, Player* player) {
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
    
    bool Settlers::activateKnight(int loc, Player* player) {
        int strength = gameBoard.getKnightLevel(loc);
        if (player->activateKnight(strength)) {
            gameBoard.activateKnight(loc);
            cout << player->getName() << " activated a level " << strength << " knight on position " << loc << endl;
            defense += strength;
            activeKnightLocs[loc] = player; // map this newly activated location to the player who owns it
            actThisTurnLocs.emplace(loc); // keep track of the fact that you activated this knight
            /* fix the strength leader order */
            int iter = 0;
            while (strengthLeaders[iter] != player) iter++; // find index of the person who just activated
            while (iter > 0 && // swap until you find his correct swap
                   strengthLeaders[iter]->getKnightStrength() < strengthLeaders[iter-1]->getKnightStrength()) {
                Player* temp(strengthLeaders[iter-1]);
                strengthLeaders[iter-1] = strengthLeaders[iter];
                strengthLeaders[iter] = temp;
                iter--;
            }
            return true;
        }
        cout << player->getName() << " could not active a knight on position " << loc << endl;
        return false;
    }
    
    bool Settlers::deactivateKnight(int loc, Player* player, bool deactivateAll) {
        gameBoard.deactivateKnight(loc); // sets that nodes knight->activated to false
        int strength = gameBoard.getKnightLevel(loc);
        defense -= strength; // decrease the current total knight strength
        player->deactivateKnight(strength); // reduce the player's personal knight strength
        activeKnightLocs.erase(activeKnightLocs.find(loc)); // remove the previously activated position from the map
        if (!deactivateAll){
            /* fix the strength leader order */
            int iter = 0;
            while (strengthLeaders[iter] != player) iter++; // find index of the person who just activated
            while (iter < strengthLeaders.size()-1 &&
                   strengthLeaders[iter]->getKnightStrength() < strengthLeaders[iter+1]->getKnightStrength()) {
                Player* temp(strengthLeaders[iter+1]);
                strengthLeaders[iter+1] = strengthLeaders[iter];
                strengthLeaders[iter] = temp;
                iter++;
            }
        }
        return true;
    }
    
    bool Settlers::buildMetropolis(string type, int loc, Player* player) {
        pair<Player*, int> currMet = metropoli[type];
        metropoli[type].first = player; // the owner of the metropolis of that type is now the player
        Metropolis* met = nullptr;
        if (currMet.first != nullptr) {
            met = gameBoard.removeMetropolis(currMet.second); // remove the metropolis from his city
            currMet.first->removeMetropolis(); // take away the player's metropolis
            canRaze[currMet.first].insert(currMet.second); // add that city to the set of vulenrable cities
        } else met = new Metropolis(type);
        gameBoard.placeMetropolis(loc, met);
        player->buildMetropolis();
        canRaze[player].erase(canRaze[player].find(loc)); // remove that city from the vulnerable cities
        metropoli[type].second = loc;
        return true;
    }
    
    void Settlers::moveRobber() {
        Player* thePlayer = players[turnTicker];
        cout << thePlayer->getName() << " gets to move the robber.\nWhere do you want to place the robber?\n";
        int loc;
        cin >> loc;
        while (!gameBoard.isValidRobberLoc(loc)) {
            cout << "Invalid robber loc.\nWhere do you want to place the robber?\n";
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
            } if (playerName == "stop") exit(1); // TODO: why am I exiting the program?
            thePlayer->rob(victim);
        }
    }
    
    void Settlers::raze(Player* player) {
        cout << "You're the weakest link and are being razed. Choose the city you prefer to get razed.\n";
        int cityLoc;
        cin >> cityLoc;
        while (!gameBoard.isOpenCityLoc(cityLoc, player)) {
            cout << "That's an invalid place to raze.\nChoose the city you prefer to get razed.\n";
            int cityLoc;
            cin >> cityLoc;
        }
        player->destroyCity();
        gameBoard.removeCity(cityLoc);
        player->buildSettlement(true); // set first turn = true so that he doesn't get charged for construction.
        Settlement* settlement = new Settlement(player);
        gameBoard.placeSettlement(cityLoc, settlement);
        canRaze[player].erase(canRaze[player].find(cityLoc)); // this city is gone so it's no longer vulnerable
    }
    
    void Settlers::resolveEvent(const string& color, int red) {
        if (color == "black") {
            barbarians.move();
            if (barbarians.location == 7) {
                if (barbarians.strength > defense) {
                    Player* weakestLink = strengthLeaders[strengthLeaders.size()-1];
                    int iter = strengthLeaders.size()-2; // the next potential weakest link index
                    while (iter >-1 && canRaze[weakestLink].size() == 0) { // find the first person that can be razed
                        weakestLink = strengthLeaders[iter];
                        iter--;
                    } if (canRaze[weakestLink].size() == 0) cout << "No one was razed.\n";
                    else { // we can raze someone
                        Player* nextWeakest = (iter >= 0) ? strengthLeaders[iter] : nullptr; // if the first person we razed was the strength leader (i.e. strengthLeader[0]), iter will be -1 and there is no potential next weakest
                        raze(weakestLink);
                        while (nextWeakest != nullptr && nextWeakest->getKnightStrength() == weakestLink->getKnightStrength()) {
                            weakestLink = nextWeakest;
                            iter--;
                            nextWeakest = (iter >= 0) ? strengthLeaders[iter] : nullptr;
                            raze(weakestLink);
                        }
                    }
                } else {
                    set<Player*> defenders;
                    defenders.insert(strengthLeaders[0]);
                    int iter = 1;
                    // get all players that have equal strength to the person defending the most
                    while (iter < players.size() &&
                           strengthLeaders[iter]->getKnightStrength() == (*defenders.begin())->getKnightStrength()) {
                        defenders.insert(strengthLeaders[iter]);
                        iter++;
                    }
                    if (defenders.size() == 1) { // there was only one leader
                        (*defenders.begin())->defendCatan(); // whoever the defender is, give him a victory point
                    } else { // there was a tie
                        int haveSelected = 0;
                        int i = 0;
                        while (haveSelected != defenders.size()) {
                            int choiceTicker = (turnTicker+i)%players.size(); // in turn order...
                            if (defenders.find(players[choiceTicker]) != defenders.end()) { // ...find the defender...
                                // ...have him choose a progress card
                                haveSelected++;
                            }
                            i++;
                        }
                    }
                }
                if (!robberInPlay) robberInPlay = true; // the first time we get attacked we set this to true
                barbarians.reset(); // they go back to their starting position
                for (auto knightLoc : activeKnightLocs) { // deactivate all knights
                    deactivateKnight(knightLoc.first, knightLoc.second, true); // knightLoc = {<int>, <Player*>}
                }
                activeKnightLocs.clear();
            }
            return;
        } else if (color == "green") { // access to 2 : 1 flip; 3 : 2; 4 : 3...
            for (int i = 0; i < players.size(); i++) {
                if (red - players[(turnTicker + i)%players.size()]->getCitImprovements("science") <= 1) {
                    // TODO: choose a progress card
                }
            }
            return;
        } else if (color == "blue") {
            for (int i = 0; i < players.size(); i++) {
                if (red - players[(turnTicker + i)%players.size()]->getCitImprovements("politics") <= 1) {
                    // TODO: choose a progress card
                }
            }
            return;
        } else if (color == "yellow") {
            for (int i = 0; i < players.size(); i++) {
                if (red - players[(turnTicker + i)%players.size()]->getCitImprovements("trade") <= 1) {
                    // TODO: choose a progress card
                }
            }
            return;
        }
    }
    
    void Settlers::resolveSeven() {
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
        if (robberInPlay) moveRobber();
    }
    
    void Settlers::resolveRoll(int roll) {
        if (roll != 7) { // collection
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
    
    void Settlers::turn() {
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
            if (action == "build") buildActions();
            if (action == "deactivate") {
                int loc;
                cout << "Where is the knight you'd like to deactivate?\n";
                cin >> loc;
                if (gameBoard.canDeactivateKnight(loc, thePlayer)) {
                    cout << "What would you like to do with that knight?\n";
                    string act;
                    cin >> act;
                    bool success = false;
                    while (!success && act != "cancel") { // cancel means they don't want to do whatever they originaly said they wanted to do. not success means they tried do something, but failed to do so.
                        if (act != "move" || act != "dispell") {
                            cout << "That action is invalid.\nWhat would you like to do with that knight?\n";
                            string act;
                            cin >> act;
                        }
                        if (act == "dispell") {
                            if (gameBoard.nodeIsNextToTile(loc, gameBoard.getRobberLoc())) {
                                moveRobber();
                                success = true;
                            } else cout << "The robber is not on your tile.\n";
                        }
                        if (act == "move") {
                            cout << "Where would you like to move it to?\n";
                            int destination;
                            cin >> destination;
                            if (gameBoard.canMoveKnight(thePlayer, loc, destination)) {
                                deactivateKnight(loc, thePlayer);
                                gameBoard.moveKnight(loc, destination);
                            }
                            success = true;
                        }
                    }
                    if (act != "cancel") { // success is true
                        deactivateKnight(loc, thePlayer);
                    }
                }
            }
            if (action == "trade") {
                // tradeActions()
            }
            if (action == "prog") {
                // how will this work?
            }
            if (thePlayer->getPoints() >= 13) return;
            cout << "What would you like to do?\n";
            cin >> action;
        }
        for (int knightLoc : actThisTurnLocs) gameBoard.removeSummonSickness(knightLoc);
        activeKnightLocs.clear();
        // TODO: implement keeping track of road chains
        // no need to keep track of them. Just keep track of the size of the longest one
        // everytime the player builds a road, after checking if it's a legal road placement, the board will run a DFS of sorts to calculate the longest path from that road until the end of its longest chain. There has to be a way to not do a DFS *every* time you build a road...
        // common, think recursively, you can do it!
        // if this length > current longest road for the player, reassign it
        // if current longest road for the player > longest road, take away longest road from the leader, give it to the current player
        // ask for a new location
        // determine if it's part of the road chain
        // determine if it's available
        // if this and the condition above it are true, consider method: gameBoard.moveKnight()
        // account for possible enemy knight displacement.
        // gameBoard.moveKnight() will factor in the possibility of displacing other knights by calling moveKnight with the enemy knight. NOTICE: moveKnight() will assume that not being able to move a knight to a new location is a possibility (thereby deleting the knight). This component of the code will have to check that the first person (the player who deactivated his knight) has the possibility of relocating his knight. Otherwise, if he doesn't and the function is called anyway, the function will determine that he does not have a new place to put his knight and will thus delete it instead of just reporting that the action was invalid to begin with.
        
        
        // NOTE: it should be the responsibility of these actions in this class to
        // check if the action is compatabile with the STATE OF THE BOARD.
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
        // trade
        // play progress card
        // end turn
    }
}
