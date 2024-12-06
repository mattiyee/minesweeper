#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;

#define DELETE_KEY 8

class WelcomeWindow{
public:
    sf::Text getName() const;
    bool getForceClosed() const;
    string getUsername() const;
    void setText(sf::Text &text, float x, float y);
    void setUsername(string text);
    sf::Text validateName(char temp, string& userInput, int rowCount, int colCount);
    void mainMenu(int rowCount, int colCount);

private:
    sf::Text welcome;
    sf::Text enterName;
    sf::Text nameInput;
    string username;
    bool forceClosed = true;
};

namespace Algorithms{
    void readConfig(const string& file, int* width, int* height, int* mines);
}
