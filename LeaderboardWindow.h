#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
using namespace std;

class LeaderboardWindow {
public:
    void boardScreen(int rowCount, int colCount);
    void boardScreen(int rowCount, int colCount, int currentMin, float currentSec, string username);
    void setText(sf::Text &text, float x, float y);

    string readLeaderboard(const string& file, sf::RenderWindow& window);
    string determineRank(const string& file, sf::RenderWindow& window, int currentMin, float currentSec, string username);
    vector<string> splitParts(string fullRead);
private:
    sf::Text title;
    sf::Text allScores;
};
