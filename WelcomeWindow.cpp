#include "WelcomeWindow.h"
using namespace Algorithms;

sf::Text WelcomeWindow::getName() const{
    return nameInput;
}

bool WelcomeWindow::getForceClosed() const{
    return forceClosed;
}

string WelcomeWindow::getUsername() const{
    return username;
}

void WelcomeWindow::setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
    // Sets text to certain position based off x and y
}
// Function positions input text on board

void WelcomeWindow::setUsername(string text){
    username = text;
}

sf::Text WelcomeWindow::validateName(char temp, string& userInput, int rowCount, int colCount){
    int window_width = colCount * 32;
    int window_height = (rowCount * 32) + 100;
    if(temp == DELETE_KEY && userInput.length() > 0){
        string newText;
        for(int i = 0; i < userInput.length() - 1; i++){
            newText += userInput[i];
        }
        userInput = newText;
        nameInput.setString(userInput + "|");
        setText(nameInput, static_cast<float>((window_width)) / 2, (static_cast<float>((window_height)) / 2) - 45);
        return nameInput;
    }
        // Handles deleting characters from the string
    else if(isalpha(temp)){
        if(userInput.length() == 0){
            userInput += (char)toupper(temp);
            nameInput.setString(userInput + "|");
            setText(nameInput, static_cast<float>((window_width)) / 2, (static_cast<float>((window_height)) / 2) - 45);
        }
        else if(userInput.length() < 10 && userInput.length() != 0){
            userInput += (char)tolower(temp);
            nameInput.setString(userInput + "|");
            setText(nameInput, static_cast<float>((window_width)) / 2, (static_cast<float>((window_height)) / 2) - 45);
        }
    }
    // Handles checking if the input is an alphabetical character, and fixes uppercase/lowercase
    setUsername(userInput);
    return nameInput;
}
// Function validates each input character of the user input

void WelcomeWindow::mainMenu(int rowCount, int colCount){
    int window_width = colCount * 32;
    int window_height = (rowCount * 32) + 100;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Minesweeper - Spring 2024", sf::Style::Close | sf::Style::Titlebar);

    sf::Font font;
    if(!font.loadFromFile("files/font.ttf")){ 
        cerr << "Font file could not load." << endl;
    }
    // Loads font into program

    welcome.setFont(font);
    welcome.setString("WELCOME TO MINESWEEPER!");
    welcome.setCharacterSize(24);
    welcome.setFillColor(sf::Color::White);
    welcome.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcome, static_cast<float>(window_width) / 2, (static_cast<float>((window_height)) / 2) - 150);
    // Text for welcome statement

    enterName.setFont(font);
    enterName.setString("Enter your name:");
    enterName.setCharacterSize(24);
    enterName.setFillColor(sf::Color::White);
    enterName.setStyle(sf::Text::Bold);
    setText(enterName, static_cast<float>((window_width)) / 2, (static_cast<float>((window_height)) / 2) - 75);
    // Text for entering name statement

    nameInput.setFont(font);
    nameInput.setString("|");
    nameInput.setCharacterSize(18);
    nameInput.setFillColor(sf::Color::Yellow);
    nameInput.setStyle(sf::Text::Bold);
    setText(nameInput, static_cast<float>((window_width)) / 2, (static_cast<float>((window_height)) / 2) - 45);
    // Original text for name line

    string userInput;
    while(window.isOpen()){
        sf::Event evnt;
        while(window.pollEvent(evnt)){
            switch(evnt.type){
                case sf::Event::Closed:
                    window.close();
                    forceClosed = true;
                    break;
                case sf::Event::TextEntered:
                    if(evnt.text.unicode < 128){
                        char temp = evnt.text.unicode;
                        validateName(temp, userInput, rowCount, colCount);
                    }
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && userInput.length() != 0){
            window.close();
            forceClosed = false;
            break;
        }
        // Transitions into the game window
        window.clear(sf::Color::Blue);
        window.draw(welcome);
        window.draw(enterName);
        window.draw(nameInput);
        window.display();
    }
}
// Function runs the main menu program

void Algorithms::readConfig(const string& file, int* width, int* height, int* mines){
    if(!(file == "config.cfg")){
        cerr << "Incorrect file name." << endl;
        return;
    }
    // Checks whether file input is correct or not.

    ifstream infile("files/config.cfg"); 
    string line;
    vector<string> fileRead;

    if(!infile.is_open()){
        cerr << "Unable to open file." << endl;
        return;
    } // Captures whether file was successfully opened or not

    while(getline(infile, line)){
        fileRead.push_back(line);
    } // While loop to read file
    infile.close();

    *width = stoi(fileRead[0]);
    *height = stoi(fileRead[1]);
    *mines = stoi(fileRead[2]);
    // Stores vector elements into pointers for use in main
}
// Function reads the config file