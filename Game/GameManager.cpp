#include "GameManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>

ScoreManager::ScoreManager(const std::string& filename)
    : filename_(filename)
{
    loadFromFile();
}

void ScoreManager::loadFromFile() {
    std::ifstream in(filename_, std::ios::binary);
    if (!in) {
        bestRecord_ = { -1, std::numeric_limits<float>::infinity() };
        return;
    }

    in.read(reinterpret_cast<char*>(&bestRecord_), sizeof(bestRecord_));
}

void ScoreManager::saveToFile() const {
    std::ofstream out(filename_, std::ios::binary | std::ios::trunc);
    out.write(reinterpret_cast<const char*>(&bestRecord_), sizeof(bestRecord_));
}

void ScoreManager::recordFinish(const PlayerRecord& result) {
    if (result.score > bestRecord_.score ||
        (result.score == bestRecord_.score && result.timeTaken < bestRecord_.timeTaken)) {
        bestRecord_ = result;
        saveToFile();
    }
}