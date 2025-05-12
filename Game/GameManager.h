#pragma once
#include <string>
#include <vector>
#include <limits>

struct PlayerRecord {
    int score;
    float timeTaken;      // in seconds
};

class ScoreManager {
private:
    std::string      filename_;
    PlayerRecord bestRecord_;

    void loadFromFile();
    void saveToFile() const;

public:
    explicit ScoreManager(const std::string& filename);
    void recordFinish(const PlayerRecord& result);
    PlayerRecord getBestRecord() const { return bestRecord_; }
};