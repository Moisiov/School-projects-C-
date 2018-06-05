#include "gamerules.hh"
#include "card.hh"
#include <iostream>

int GameRules::getHandValue(std::vector<std::vector<int> > handData)
{
    int value = 9;

    if(straight(handData)) {
        value = 5;
    }
    if(flush(handData)) {
        if(value == 5) {
            value = 1;
        } else {
            value = 4;
        }
    }

    std::vector<int> pairsData = pairs(handData);
    unsigned int pairsDataSize = pairsData.size();
    if(pairsDataSize != 0) {
        switch (pairsDataSize) {
        case 1:
            value = 8;
            break;
        case 2:
            value = 7;
            break;
        case 3:
            value = 6;
            break;
        case 4:
            value = 3;
            break;
        case 6:
            value = 2;
            break;
        }
    }
    return value;
}

bool GameRules::flush(std::vector<std::vector<int> > handData)
{
    int suit = handData.at(0).at(1);
    for(int i = 1; i < 5; ++i) {
        if(handData.at(i).at(1) != suit) {
            return false;
        }
    }

    return true;
}

bool GameRules::straight(std::vector<std::vector<int> > handData)
{
    std::vector<int> values = {};
    for(int i = 0; i < 5; ++i) {
        values.push_back(handData.at(i).at(0));
    }

    std::sort(values.begin(), values.end());
    for(int i = 0; i < 4; ++i) {
        if(values.at(i) != values.at(i+1) - 1) {
            return false;
        }
    }

    return true;
}

std::vector<int> GameRules::pairs(std::vector<std::vector<int> > handData)
{
    std::vector<int> pairsData = {};
    for(int i = 0; i < 4; ++i) {
        for(int j = i+1; j < 5; ++j) {
            if(handData.at(i).at(0) == handData.at(j).at(0)) {
                pairsData.push_back(handData.at(i).at(0));
            }
        }
    }
    return pairsData;
}

int GameRules::high(std::vector<int>  handData)
{
    int value = handData.at(0);
    for(unsigned int i = 1; i < handData.size(); ++i) {
        if(handData.at(i) > value) {
            value = handData.at(i);
        }
    }

    return value;
}

std::vector<int> GameRules::getHighVec(std::vector<std::vector<int>> handData,
                                       std::vector<int> cardsInPair)
{
    std::vector<int> handHigh = {};
    for(int i = 0; i < 5; ++i) {
        if(std::find(cardsInPair.begin(), cardsInPair.end(),
                     handData.at(i).at(0)) == cardsInPair.end()) {
            handHigh.push_back(handData.at(i).at(0));
        }
    }
    return handHigh;
}

int GameRules::getWinner(std::vector<std::vector<int>> playerData,
                          std::vector<std::vector<int>> aiData)
{
    int playerHandValue = getHandValue(playerData);
    int aiHandValue = getHandValue(aiData);

    // Tarkastellaan tapaukset, joissa pelaajilla on samanlaiset kädet
    if(playerHandValue == aiHandValue) {
        std::vector<int> playerPairs = pairs(playerData);
        std::vector<int> playerHigh = getHighVec(playerData, playerPairs);
        std::vector<int> aiPairs = pairs(aiData);
        std::vector<int> aiHigh = getHighVec(aiData, aiPairs);

        if(playerHandValue == 4 || playerHandValue == 9 ||
                playerHandValue == 1 || playerHandValue == 5) {
            if(high(playerHigh) > high(aiHigh)) {
                return 1;
            }
            else if(high(playerHigh) == high(aiHigh)) {
                return 2;
            }
            return 0;
        }
        else if(playerHandValue == 8 || playerHandValue == 7 ||
                playerHandValue == 6 || playerHandValue == 2) {
            if(high(playerPairs) > high(aiPairs)) {
                return 1;
            }
            if(high(playerPairs) < high(aiPairs)) {
                return 0;
            }
            if(high(playerPairs) == high(aiPairs) && playerHandValue == 7) {
                if(std::min(playerPairs.at(0), playerPairs.at(1)) >
                        std::min(aiPairs.at(0), aiPairs.at(1))) {
                    return 1;
                }
                if(std::min(playerPairs.at(0), playerPairs.at(1)) <
                        std::min(aiPairs.at(0), aiPairs.at(1))) {
                    return 0;
                }
            }
            if(high(playerHigh) > high(aiHigh)) {
                return 1;
            }
            if(high(playerHigh) == high(aiHigh)) {
                return 2;
            }
            return 0;
        }
        // Full House tasatilanteessa
        if(playerHandValue == 3) {
            int playerCardsValue = playerPairs.at(0);
            if(std::find(playerPairs.begin(), playerPairs.end(), playerCardsValue)
                    == playerPairs.end())  {
                playerCardsValue = playerPairs.at(1);
            }

            int aiCardsValue = aiPairs.at(0);
            if(std::find(aiPairs.begin(), aiPairs.end(), aiCardsValue)
                    == playerPairs.end())  {
                aiCardsValue = aiPairs.at(1);
            }
            if(playerCardsValue > aiCardsValue) {
                return 1;
            }
            if(playerCardsValue == aiCardsValue) {
                return 2;
            }
            return 0;
        }
    }


    if(playerHandValue < aiHandValue) {
        return 1;
    }
    return 0;
}
