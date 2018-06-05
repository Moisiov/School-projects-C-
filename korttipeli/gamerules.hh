#ifndef GAMERULES_HH
#define GAMERULES_HH
#include <string>
#include <vector>
#include <algorithm>

namespace GameRules {

    int getHandValue(std::vector<std::vector<int>> handData);
    int getWinner(std::vector<std::vector<int>> playerData,
                   std::vector<std::vector<int>> aiData);
    std::vector<int> getHighVec(std::vector<std::vector<int>> handData,
                                std::vector<int> cardsInPair = {});

    bool flush(std::vector<std::vector<int>> handData);
    bool straight(std::vector<std::vector<int>> handData);
    std::vector<int> pairs(std::vector<std::vector<int>> handData);
    int high(std::vector<int> handData);

}

#endif // GAMERULES_HH
