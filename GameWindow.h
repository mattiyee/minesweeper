#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
using namespace std;

struct Tile{
    unsigned int x;
    unsigned int y;
    int minesAround = 0;
    bool isHidden = true; // True: Hidden, False: Revealed
    bool isMine = false;
    bool hasFlag = false;
};

class GameWindow {
public:
    GameWindow(sf::Text username, string stringName, float timer);
    void setText(sf::Text &text, float x, float y);
    int getTileIndex(int x, int y, int colCount);
    void initializeBoard(Tile* tiles, int rowCount, int colCount, int mineCount);
    void initializeMinesAround(int rowCount, int colCount);
    void gameScreen(int rowCount, int colCount, int mineCount);
    // Functions

    bool topValid(Tile* tiles, int current, int colCount);
    bool leftValid(Tile* tiles, int current);
    bool rightValid(Tile* tiles, int current, int colCount);
    bool bottomValid(Tile* tiles, int current, int rowCount, int colCount);
    bool tleftValid(Tile* tiles, int current, int colCount);
    bool trightValid(Tile* tiles, int current, int colCount);
    bool bleftValid(Tile* tiles, int current, int rowCount, int colCount);
    bool brightValid(Tile* tiles, int current, int rowCOunt, int colCount);

private:
    sf::Text username; // Name of player
    string stringName;
    float timer;

    Tile* allTiles;
    vector<vector<int>> allMines;
    int flagsRemaining; // Amount of flags remaining (counter)
};