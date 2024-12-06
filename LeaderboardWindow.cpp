#include "LeaderboardWindow.h"

void LeaderboardWindow::boardScreen(int rowCount, int colCount){
    int window_width = colCount * 16;
    int window_height = (rowCount * 16) + 50;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Minesweeper - Spring 2024", sf::Style::Close | sf::Style::Titlebar);

    sf::Font font;
    if(!font.loadFromFile("files/font.ttf")){ 
        cerr << "Font file could not load." << endl;
    }
    // Loads font into program

    title.setFont(font);
    title.setString("LEADERBOARD");
    title.setCharacterSize(20);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(title, static_cast<float>(window_width) / 2, ((static_cast<float>(window_height) / 2) - 120));

    string scores = readLeaderboard("leaderboard.txt", window);
    allScores.setFont(font);
    allScores.setString(scores);
    allScores.setCharacterSize(18);
    allScores.setFillColor(sf::Color::White);
    allScores.setStyle(sf::Text::Bold);
    setText(allScores, static_cast<float>(window_width) / 2, (static_cast<float>(window_height) / 2 + 20));

    while(window.isOpen()){
        sf::Event evnt;
        while(window.pollEvent(evnt)){
            switch(evnt.type){
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }
        window.clear(sf::Color::Blue);
        window.draw(title);
        window.draw(allScores);
        window.display();
    }
}
// Function runs the leaderboard program while the game is ongoing

void LeaderboardWindow::boardScreen(int rowCount, int colCount, int currentMin, float currentSec, string username){
    int window_width = colCount * 16;
    int window_height = (rowCount * 16) + 50;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Minesweeper - Spring 2024", sf::Style::Close | sf::Style::Titlebar);

    sf::Font font;
    if(!font.loadFromFile("files/font.ttf")){ 
        cerr << "Font file could not load." << endl;
    }
    // Loads font into program

    title.setFont(font);
    title.setString("LEADERBOARD");
    title.setCharacterSize(20);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(title, static_cast<float>(window_width) / 2, ((static_cast<float>(window_height) / 2) - 120));

    string scores = determineRank("leaderboard.txt", window, currentMin, currentSec, username);
    allScores.setFont(font);
    allScores.setString(scores);
    allScores.setCharacterSize(18);
    allScores.setFillColor(sf::Color::White);
    allScores.setStyle(sf::Text::Bold);
    setText(allScores, static_cast<float>(window_width) / 2, (static_cast<float>(window_height) / 2 + 20));

    while(window.isOpen()){
        sf::Event evnt;
        while(window.pollEvent(evnt)){
            switch(evnt.type){
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }
        window.clear(sf::Color::Blue);
        window.draw(title);
        window.draw(allScores);
        window.display();
    }
}

void LeaderboardWindow::setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
    // Sets text to certain position based off x and y
}
// Function positions input text on board

string LeaderboardWindow::readLeaderboard(const string& file, sf::RenderWindow& window){
    if(!(file == "leaderboard.txt")){
        cerr << "Incorrect file name." << endl;
        return nullptr;
    }
    // Checks whether file input is correct or not

    ifstream infile("files/leaderboard.txt"); 
    string line;
    vector<string> fileRead;

    if(!infile.is_open()){
        cerr << "Unable to open file." << endl;
        return nullptr;
    }
    // Checks whether the file was opened

    string scores;
    int rank = 1;
    while(getline(infile, line)){
        if(rank <= 5){
            vector<string> temp = splitParts(line);

            scores.append(to_string(rank) + ".\t");
            scores.append(temp[0] + ":" + temp[1] + "\t");
            scores.append(temp[2] + "\n\n");
            rank++;
        }
    }
    return scores;
}

string LeaderboardWindow::determineRank(const string& file, sf::RenderWindow& window, int currentMin, float currentSec, string username){
    if(!(file == "leaderboard.txt")){
        cerr << "Incorrect file name." << endl;
        return nullptr;
    }
    // Checks whether file input is correct or not

    ifstream infile("files/leaderboard.txt"); 
    string line;
    vector<string> fileRead;

    if(!infile.is_open()){
        cerr << "Unable to open file." << endl;
        return nullptr;
    }
    // Checks whether the file was opened

    string scores;
    int rank = 1;
    int userSeconds = 20;
    bool foundRank = false;
    while(getline(infile, line)){
        if(rank <= 5){
            vector<string> temp = splitParts(line);
            int seconds = (stoi(temp[0]) * 60) + (stoi(temp[1]));

            if(userSeconds > seconds || foundRank){
                scores.append(to_string(rank) + ".\t");
                scores.append(temp[0] + ":" + temp[1] + "\t");
                scores.append(temp[2] + "\n\n");
            }
            else if(userSeconds < seconds){
                scores.append(to_string(rank) + ".\t");
                if(currentMin <= 9){
                    scores.append("0" + to_string(currentMin) + ":");
                }
                else{
                    scores.append(to_string(currentMin) + ":");
                }
                if(currentSec <= 9){
                    scores.append("0" + to_string((int)currentSec) + "\t");
                }
                else{
                    scores.append(to_string((int)currentSec) + "\t");
                }
                scores.append(username + "*\n\n");
                foundRank = true;
            }
            rank++;
        }
    }
    return scores;
}

vector<string> LeaderboardWindow::splitParts(string fullRead){
    vector<string> res;
    int position = 0;
    while(position < fullRead.size()){
        position = fullRead.find(':');
        res.push_back(fullRead.substr(0, position));
        fullRead.erase(0, position + 1);
        cout << res[0] << " ";

        position = fullRead.find(',');
        res.push_back(fullRead.substr(0, position));
        fullRead.erase(0, position + 1);
        cout << res[1] << " ";

        position = fullRead.find('\n');
        res.push_back(fullRead.substr(1, position));
        fullRead.erase(0, position + 1);
        cout << res[2] << endl;
    }
    return res;
}