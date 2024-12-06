#include "GameWindow.h"
#include "LeaderboardWindow.h"

GameWindow::GameWindow(sf::Text name, string text, float time){
    this->username = name;
    this->stringName = text;
    this->timer = time;
}

void GameWindow::setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
    // Sets text to certain position based off x and y
}
// Function positions input text on board

int GameWindow::getTileIndex(int x, int y, int colCount){
    if(y == 1){
        return x;
    }
    else{
        return ((y - 1) * colCount) + x;
    }
}
// Finds the tile index of the given tile's coordinates

void GameWindow::initializeBoard(Tile* tiles, int rowCount, int colCount, int mineCount){
    tiles[0].x = 0;
    tiles[0].y = 0;
    // Creates an empty tile to make counting easier

    int current_tile = 1;
    for(int i = 1; i < rowCount + 1; i++){
        for(int j = 1; j < colCount + 1; j++){
            tiles[current_tile].x = j;
            tiles[current_tile].y = i;
            tiles[current_tile].minesAround = 0;
            tiles[current_tile].isHidden = true; // True: Hidden, False: Revealed
            tiles[current_tile].isMine = false;
            tiles[current_tile].hasFlag = false;
            current_tile++;
        }
    }
    // Parameterized constructor for all tiles on the board

    mt19937 rd(time(nullptr));
    uniform_int_distribution<int> x_random(1, colCount);
    uniform_int_distribution<int> y_random(1, rowCount);
    // Creates the random number generator

    int currentProduced = 0;
    while(currentProduced < mineCount){
        int mine_x = x_random(rd);
        int mine_y = y_random(rd);
        int tempIndex = getTileIndex(mine_x, mine_y, colCount);

        if(!(tiles[tempIndex].isMine)){
            tiles[tempIndex].isMine = true;
            tiles[tempIndex].isHidden = false;
            currentProduced++;
            // cout << "Mine " << currentProduced << ": " << tiles[tempIndex].x << ", " << tiles[tempIndex].y << endl;
        }
    }
    // Generates mines randomly based off limit
}
// Creates a fresh board

void GameWindow::initializeMinesAround(int rowCount, int colCount){
    int current_tile = 1;
    for(int i = 1; i < rowCount + 1; i++){
        for(int j = 1; j < colCount + 1; j++){
            if(allTiles[current_tile].isMine){
                // cout << "Mine at " << current_tile << endl;

                if(tleftValid(allTiles, current_tile, colCount)){
                    int topLeft = current_tile - colCount - 1;
                    allTiles[topLeft].minesAround++;
                    // cout << "- " << topLeft << ": " << allTiles[topLeft].minesAround << endl;
                }
                // Adds count to left-top square

                if(topValid(allTiles, current_tile, colCount)){
                    int topMiddle = current_tile - colCount;
                    allTiles[topMiddle].minesAround++;
                    // cout << "- " << topMiddle << ": " << allTiles[topMiddle].minesAround << endl;
                }
                // Adds count to left-middle square

                if(trightValid(allTiles, current_tile, colCount)){
                    int topRight = current_tile - colCount + 1;
                    allTiles[topRight].minesAround++;
                    // cout << "- " << topRight << ": " << allTiles[topRight].minesAround << endl;
                }
                // Adds count to left-right square

                if(leftValid(allTiles, current_tile)){
                    int middleLeft = current_tile - 1;
                    allTiles[middleLeft].minesAround++;
                    // cout << "- " << middleLeft << ": " << allTiles[middleLeft].minesAround << endl;
                }
                // Adds count to middle-left square

                if(rightValid(allTiles, current_tile, colCount)){
                    int middleRight = current_tile + 1;
                    allTiles[middleRight].minesAround++;
                    // cout << "- " << middleRight << ": " << allTiles[middleRight].minesAround << endl;
                }
                // Adds count to middle-right square

                if(bleftValid(allTiles, current_tile, rowCount, colCount)){
                    int bottomLeft = current_tile + colCount - 1;
                    allTiles[bottomLeft].minesAround++;
                    // cout << "- " << bottomLeft << ": " << allTiles[bottomLeft].minesAround << endl;
                }
                // Adds count to bottom-top square

                if(bottomValid(allTiles, current_tile, rowCount, colCount)){
                    int bottomMiddle = current_tile + colCount;
                    allTiles[bottomMiddle].minesAround++;
                    // cout << "- " << bottomMiddle << ": " << allTiles[bottomMiddle].minesAround << endl;
                }
                // Adds count to bottom-middle square

                if(brightValid(allTiles, current_tile, rowCount, colCount)){
                    int bottomRight = current_tile + colCount + 1;
                    allTiles[bottomRight].minesAround++;
                    // cout << "- " << bottomRight << ": " << allTiles[bottomRight].minesAround << endl;
                }
                // cout << endl;
                // Adds count to bottom-right square
            }
            current_tile++;
        }
    }
}

void GameWindow::gameScreen(int rowCount, int colCount, int mineCount){
    int window_width = colCount * 32;
    int window_height = (rowCount * 32) + 100;
    int numOfTiles = (rowCount * colCount) + 1;
    allTiles = new Tile[numOfTiles];
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Minesweeper - Spring 2024", sf::Style::Close | sf::Style::Titlebar);
    // Initializing variables

    sf::Clock clock;
    int currentMin = 0;
    float currentSec = 0;
    int pauseTime = 0;
    window.setFramerateLimit(60);
    // Initializing timer

    sf::Font font;
    if(!font.loadFromFile("files/font.ttf")){ 
        cerr << "Font file could not load." << endl;
    }
    // Initializing fonts

    sf::Texture numberOne;
    numberOne.loadFromFile("files/images/number_1.png"); 
    sf::Texture numberTwo;
    numberTwo.loadFromFile("files/images/number_2.png"); 
    sf::Texture numberThree;
    numberThree.loadFromFile("files/images/number_3.png"); 
    sf::Texture numberFour;
    numberFour.loadFromFile("files/images/number_4.png"); 
    sf::Texture numberFive;
    numberFive.loadFromFile("files/images/number_5.png"); 
    sf::Texture numberSix;
    numberSix.loadFromFile("files/images/number_6.png"); 
    sf::Texture numberSeven;
    numberSeven.loadFromFile("files/images/number_7.png"); 
    sf::Texture numberEight;
    numberEight.loadFromFile("files/images/number_8.png"); 
    // Initializing number textures

    sf::Texture hiddenTexture;
    hiddenTexture.loadFromFile("files/images/tile_hidden.png"); 
    sf::Texture revealedTexture;
    revealedTexture.loadFromFile("files/images/tile_revealed.png"); 
    sf::Texture mineTexture;
    mineTexture.loadFromFile("files/images/mine.png"); 
    sf::Texture flagTexture;
    flagTexture.loadFromFile("files/images/flag.png"); 
    sf::Sprite tile;
    tile.setTexture(hiddenTexture);
    // Initializing main tile textures and sprite

    auto digit_x = 33.0f;
    float timerMin_x = (float)(colCount * 32) - 97;
    float timerSec_x = (float)(colCount * 32) - 54;
    auto digit_y = 32 * (rowCount + 0.5) + 16;
    sf::Texture digitTexture;
    digitTexture.loadFromFile("files/images/digits.png"); 
    sf::Sprite digit;
    digit.setTexture(digitTexture);
    // Initializing counter and timer textures

    auto face_x = static_cast<float>((colCount / 2) * 32) - 32;
    auto face_y = static_cast<float>(32 * (rowCount + 0.5));
    sf::Texture faceTexture;
    faceTexture.loadFromFile("files/images/face_happy.png"); 
    sf::Texture winTexture;
    winTexture.loadFromFile("files/images/face_win.png");
    sf::Texture loseTexture;
    loseTexture.loadFromFile("files/images/face_lose.png");
    sf::Sprite face;
    face.setTexture(faceTexture);
    face.setPosition(face_x, face_y);
    // Initializing face textures (happy, victory, lose)

    auto debug_x = static_cast<float>((colCount * 32) - 304);
    auto debug_y = static_cast<float>(32 * (rowCount + 0.5));
    sf::Texture debugTexture;
    debugTexture.loadFromFile("files/images/debug.png"); 
    sf::Sprite debug;
    debug.setTexture(debugTexture);
    debug.setPosition(debug_x, debug_y);
    // Initializing debug texture

    auto status_x = static_cast<float>((colCount * 32) - 240);
    auto status_y = static_cast<float>(32 * (rowCount + 0.5));
    sf::Texture statusPause;
    statusPause.loadFromFile("files/images/pause.png"); 
    sf::Texture statusPlay;
    statusPlay.loadFromFile("files/images/play.png"); 
    sf::Sprite status;
    status.setTexture(statusPause);
    status.setPosition(status_x, status_y);
    // Initializing pause and play textures

    LeaderboardWindow board_screen;
    auto leader_x = static_cast<float>((colCount * 32) - 176);
    auto leader_y = static_cast<float>(32 * (rowCount + 0.5));
    sf::Texture leaderboardTexture;
    leaderboardTexture.loadFromFile("files/images/leaderboard.png"); 
    sf::Sprite leaderboard;
    leaderboard.setTexture(leaderboardTexture);
    leaderboard.setPosition(leader_x, leader_y);
    // Initializing leaderboard texture

    bool gameOver = false;
    bool gameWon = false;
    bool gamePaused = false;
    bool inDebug = false;
    bool inLeaderboard = false;
    flagsRemaining = mineCount;
    initializeBoard(allTiles, rowCount, colCount, mineCount);
    initializeMinesAround(rowCount, colCount);
    // Creating the board

    while(window.isOpen()){
        sf::Event evnt;
        while(window.pollEvent(evnt)){
            switch(evnt.type){
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::MouseButtonReleased:
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    int mouse_x = (mousePosition.x / 32) + 1;
                    int mouse_y = (mousePosition.y / 32) + 1;
                    int tileCount = getTileIndex(mouse_x, mouse_y, colCount);
                    // Handles coordinate system

                    switch(evnt.key.code){
                        case sf::Mouse::Left:
                            if(face_x <= (float)(mousePosition.x) && (float)(mousePosition.x) <= (face_x + 64)){
                                if(face_y <= (float)(mousePosition.y) && (float)(mousePosition.y) <= (face_y + 64)){
                                    initializeBoard(allTiles, rowCount, colCount, mineCount);
                                    initializeMinesAround(rowCount, colCount);
                                    face.setTexture(faceTexture);
                                    face.setPosition(face_x, face_y);

                                    gameOver = false;
                                    gameWon = false;
                                    gamePaused = false;
                                    flagsRemaining = mineCount;

                                    clock.restart();
                                    currentMin = 0;
                                    currentSec = 0;
                                    pauseTime = 0;
                                }
                            }
                            // Code for pressing the face button

                            if(status_x <= (float)(mousePosition.x) && (float)(mousePosition.x) <= (status_x + 64)){
                                if(status_y <= (float)(mousePosition.y) && (float)(mousePosition.y) <= (status_y + 64)){
                                    if(!gameOver){
                                        if(!gamePaused){
                                            gamePaused = true;
                                        }
                                        else{
                                            gamePaused = false;
                                        }
                                    }
                                }
                            }
                            // Code for pressing the pause/play button

                            if(leader_x <= (float)(mousePosition.x) && (float)(mousePosition.x) <= (leader_x + 64)){
                                if(leader_y <= (float)(mousePosition.y) && (float)(mousePosition.y) <= (leader_y + 64)){
                                    if(!gameOver){
                                        inLeaderboard = true;
                                        gamePaused = true;
                                    }
                                    else if(gameOver){
                                        if(gameWon){
                                            board_screen.boardScreen(rowCount, colCount, currentMin, floor(currentSec), stringName);
                                        }
                                        else{
                                            board_screen.boardScreen(rowCount, colCount);
                                        }
                                    }
                                    
                                    break;
                                }
                            }
                            // Code for pressing the leaderboard button

                            if(!gamePaused || !gameOver){
                                if(0 <= (float)(mousePosition.x) && (float)(mousePosition.x) <= (float)(colCount * 32)){
                                    if(0 <= (float)(mousePosition.y) && (float)(mousePosition.y) <= (float)(rowCount * 32)){
                                        if(allTiles[tileCount].isMine){
                                            gameOver = true;
                                            gameWon = false;
                                        } // Mine tile -> Game over
                                        else if(allTiles[tileCount].isHidden && allTiles[tileCount].hasFlag){
                                            continue;
                                        } // Hidden tile and has a flag -> Bypass
                                        else if(allTiles[tileCount].hasFlag){
                                            allTiles[tileCount].isHidden = true;
                                            continue;
                                        } // Flag tile -> Bypass
                                        else if(allTiles[tileCount].isHidden){
                                            allTiles[tileCount].isHidden = false;
                                        } // Hidden tile -> Not hidden tile

                                        cout << mouse_x << ", " << mouse_y << endl;
                                    }
                                }
                                // Code for pressing tiles

                                if(debug_x <= (float)(mousePosition.x) && (float)(mousePosition.x) <= (debug_x + 64)){
                                    if(debug_y <= (float)(mousePosition.y) && (float)(mousePosition.y) <= (debug_y + 64)){
                                        if(!inDebug){
                                            inDebug = true;
                                        }
                                        else{
                                            inDebug = false;
                                        }
                                    }
                                }
                                // Code for pressing the debug button
                                break;
                            }

                        case sf::Mouse::Right:
                            if(!gamePaused){
                                if(!(allTiles[tileCount].hasFlag)){
                                    allTiles[tileCount].isHidden = true;
                                    allTiles[tileCount].hasFlag = true;
                                    flagsRemaining--;
                                }
                                    // Doesn't have a flag -> Has a flag

                                else if(allTiles[tileCount].hasFlag){
                                    allTiles[tileCount].isHidden = true;
                                    allTiles[tileCount].hasFlag = false;
                                    flagsRemaining++;
                                }
                                // Has a flag -> Doesn't have a flag
                                break;
                            }
                    }
            }
        }
        window.clear(sf::Color::White);

        if(gameOver && gameWon){
            face.setTexture(winTexture);
            face.setPosition(face_x, face_y);
            gamePaused = true;
        }
        else if(gameOver && !gameWon){
            face.setTexture(loseTexture);
            face.setPosition(face_x, face_y);
            gamePaused = true;
        }

        if(!gamePaused){
            status.setTexture(statusPause);
            status.setPosition(status_x, status_y);
        }
        else{
            status.setTexture(statusPlay);
            status.setPosition(status_x, status_y);
        }
        // Logic for displaying pause/play buttons

        if(!gamePaused){
            currentSec = clock.getElapsedTime().asSeconds() + (float)pauseTime;
            if(currentSec > 60){
                currentMin++;
                pauseTime = 0;
                clock.restart();
            }
            timer = (float)(currentMin * 60) + currentSec;
            // Converts seconds to minutes

            if(currentSec < 10){
                digit.setTextureRect(sf::IntRect(0, 0, 21, 32));
                digit.setPosition(timerSec_x, (float)digit_y);
                window.draw(digit);
                digit.setTextureRect(sf::IntRect((int)(21 * floor(currentSec)), 0, 21, 32));
                digit.setPosition(timerSec_x + 21, (float)digit_y);
                window.draw(digit);
            }
            else if(currentSec >= 10 && currentSec < 60){
                int tensDigit = (int)(floor(currentSec) / 10) % 10;
                int onesDigit = (int)(floor(currentSec)) - (tensDigit * 10);
                digit.setTextureRect(sf::IntRect((int)(21 * floor(tensDigit)), 0, 21, 32));
                digit.setPosition(timerSec_x, (float)digit_y);
                window.draw(digit);
                digit.setTextureRect(sf::IntRect((int)(21 * floor(onesDigit)), 0, 21, 32));
                digit.setPosition(timerSec_x + 21, (float)digit_y);
                window.draw(digit);
            }
            // Logic for displaying seconds for timer

            if(currentMin < 10){
                digit.setTextureRect(sf::IntRect(0, 0, 21, 32));
                digit.setPosition(timerMin_x, (float)digit_y);
                window.draw(digit);
                digit.setTextureRect(sf::IntRect((int)(21 * floor(currentMin)), 0, 21, 32));
                digit.setPosition(timerMin_x + 21, (float)digit_y);
                window.draw(digit);
            }
            else if(currentMin < 60){
                int tensDigit = (int)(floor(currentMin) / 10) % 10;
                int onesDigit = (int)(floor(currentMin)) - (tensDigit * 10);
                digit.setTextureRect(sf::IntRect((int)(21 * floor(tensDigit)), 0, 21, 32));
                digit.setPosition(timerMin_x, (float)digit_y);
                window.draw(digit);
                digit.setTextureRect(sf::IntRect((int)(21 * floor(onesDigit)), 0, 21, 32));
                digit.setPosition(timerMin_x + 21, (float)digit_y);
                window.draw(digit);
            }
            // Logic for displaying minutes for timer
        }
        else if(gamePaused || gameOver || inLeaderboard){
            pauseTime = currentSec;
            clock.restart();
            if(pauseTime < 10){
                digit.setTextureRect(sf::IntRect(0, 0, 21, 32));
                digit.setPosition(timerSec_x, digit_y);
                window.draw(digit);
                digit.setTextureRect(sf::IntRect((int)(21 * pauseTime), 0, 21, 32));
                digit.setPosition(timerSec_x + 21, digit_y);
                window.draw(digit);
            }
            else if(pauseTime >= 10 && pauseTime < 60){
                int tensDigit = (int)(floor(pauseTime) / 10) % 10;
                int onesDigit = floor(pauseTime) - (tensDigit * 10);
                digit.setTextureRect(sf::IntRect((21 * tensDigit), 0, 21, 32));
                digit.setPosition(timerSec_x, digit_y);
                window.draw(digit);
                digit.setTextureRect(sf::IntRect((21 * onesDigit), 0, 21, 32));
                digit.setPosition(timerSec_x + 21, digit_y);
                window.draw(digit);
            }
            // Logic for displaying seconds for timer

            if(currentMin < 10){
                digit.setTextureRect(sf::IntRect(0, 0, 21, 32));
                digit.setPosition(timerMin_x, digit_y);
                window.draw(digit);
                digit.setTextureRect(sf::IntRect((21 * floor(currentMin)), 0, 21, 32));
                digit.setPosition(timerMin_x + 21, digit_y);
                window.draw(digit);
            }
            else if(currentMin >= 10 && currentMin < 60){
                int tensDigit = (int)(floor(currentMin) / 10) % 10;
                int onesDigit = floor(currentMin) - (tensDigit * 10);
                digit.setTextureRect(sf::IntRect((21 * floor(tensDigit)), 0, 21, 32));
                digit.setPosition(timerMin_x, digit_y);
                window.draw(digit);
                digit.setTextureRect(sf::IntRect((21 * floor(onesDigit)), 0, 21, 32));
                digit.setPosition(timerMin_x + 21, digit_y);
                window.draw(digit);
            }
            // Logic for displaying minutes for timer
        }
        // Logic for displaying clock

        string currentCount = to_string(abs(flagsRemaining));
        int length = currentCount.size();
        if(flagsRemaining < 0){
            digit.setTextureRect(sf::IntRect(210, 0, 21, 32));
            digit.setPosition(12, digit_y);
            window.draw(digit);
        }
        // Logic if the amount of flags remaining is negative

        if(length == 1){
            digit.setTextureRect(sf::IntRect(0, 0, 21, 32));
            digit.setPosition(digit_x, digit_y);
            window.draw(digit);
            digit.setTextureRect(sf::IntRect(0, 0, 21, 32));
            digit.setPosition(digit_x + 21, digit_y);
            window.draw(digit);
            digit.setTextureRect(sf::IntRect((21 * abs(flagsRemaining)), 0, 21, 32));
            digit.setPosition(digit_x + 42, digit_y);
            window.draw(digit);
        }
        // Logic if the amount of flags remaining is a single digit

        else if(length == 2){
            int tensDigit = (abs(flagsRemaining) / 10) % 10;
            int onesDigit = abs(flagsRemaining) - (tensDigit * 10);
            digit.setTextureRect(sf::IntRect(0, 0, 21, 32));
            digit.setPosition(digit_x, digit_y);
            window.draw(digit);
            digit.setTextureRect(sf::IntRect((21 * tensDigit), 0, 21, 32));
            digit.setPosition(digit_x + 21, digit_y);
            window.draw(digit);
            digit.setTextureRect(sf::IntRect((21 * onesDigit), 0, 21, 32));
            digit.setPosition(digit_x + 42, digit_y);
            window.draw(digit);
        }
        // Logic if the amount of flags remaining is double digits

        else if(length == 3){
            int hundredDigit = (abs(flagsRemaining) / 100) % 10;
            int tensDigit = (abs(flagsRemaining) / 10) % 10;
            int onesDigit = abs(flagsRemaining) - (hundredDigit * 100) - (tensDigit * 10);
            digit.setTextureRect(sf::IntRect((21 * hundredDigit), 0, 21, 32));
            digit.setPosition(digit_x, digit_y);
            window.draw(digit);
            digit.setTextureRect(sf::IntRect((21 * tensDigit), 0, 21, 32));
            digit.setPosition(digit_x + 21, digit_y);
            window.draw(digit);
            digit.setTextureRect(sf::IntRect((21 * onesDigit), 0, 21, 32));
            digit.setPosition(digit_x + 42, digit_y);
            window.draw(digit);
        }
        // Logic if the amount of flags remaining is triple digits

        int tileCount = 0;
        int correctMines = 0;

        if(inLeaderboard){
            for(int i = 0; i < rowCount; i++) {
                for (int j = 0; j < colCount; j++) {
                    tile.setTexture(revealedTexture);
                    tile.setPosition(static_cast<float>((j * 32)), static_cast<float>(i * 32));
                    window.draw(tile);
                }
            }
            window.draw(face);
            window.draw(debug);
            window.draw(status);
            window.draw(leaderboard);
            window.display();
            board_screen.boardScreen(rowCount, colCount);
            inLeaderboard = false;
            gamePaused = false;
        }
        else if(!inLeaderboard){
            for(int i = 0; i < rowCount; i++){
                for(int j = 0; j < colCount; j++){
                    tileCount++;
                    if(allTiles[tileCount].isMine && allTiles[tileCount].hasFlag){
                        correctMines++;
                        if(correctMines == mineCount){
                            gameOver = true;
                            gameWon = true;
                            gamePaused = true;
                        }
                    }

                    if(gameOver || inDebug){
                        if(allTiles[tileCount].isMine){
                            allTiles[tileCount].isHidden = false;
                            tile.setTexture(revealedTexture);
                            tile.setPosition(static_cast<float>((j * 32)), static_cast<float>(i * 32));
                            window.draw(tile);
                            tile.setTexture(mineTexture);
                            tile.setPosition(static_cast<float>((j * 32)), static_cast<float>(i * 32));
                            window.draw(tile);
                            continue;
                        }
                    }

                    if(gamePaused && !gameOver){
                        tile.setTexture(hiddenTexture);
                        tile.setPosition(static_cast<float>((j * 32)), static_cast<float>(i * 32));
                        window.draw(tile);
                    }
                    else{
                        if(allTiles[tileCount].hasFlag){
                            tile.setTexture(hiddenTexture);
                            tile.setPosition(static_cast<float>((j * 32)), static_cast<float>(i * 32));
                            window.draw(tile);
                            tile.setTexture(flagTexture);
                            tile.setPosition(static_cast<float>((j * 32)), static_cast<float>(i * 32));
                            window.draw(tile);
                            continue;
                        }
                            // Logic if the tile has a flag placed on it

                        else if(allTiles[tileCount].isHidden || allTiles[tileCount].isMine){
                            if(allTiles[tileCount].hasFlag){
                                continue;
                            }
                            else{
                                tile.setTexture(hiddenTexture);
                                tile.setPosition(static_cast<float>((j * 32)), static_cast<float>(i * 32));
                                window.draw(tile);
                            }
                        }
                            // Logic if the tile is hidden

                        else if(!(allTiles[tileCount].isMine) && !(allTiles[tileCount].isHidden)){
                            if(allTiles[tileCount].minesAround == 0){
                                tile.setTexture(revealedTexture);
                                tile.setPosition(static_cast<float>((j * 32)), static_cast<float>(i * 32));
                                window.draw(tile);
                            }
                            // Logic if tile is not a mine with no nearby mines, but is revealed

                            if(allTiles[tileCount].minesAround != 0 && !(allTiles[tileCount].isMine)){
                                tile.setTexture(revealedTexture);
                                tile.setPosition(static_cast<float>((j * 32)), static_cast<float>(i * 32));
                                window.draw(tile);

                                if(allTiles[tileCount].minesAround == 1){
                                    tile.setTexture(numberOne);
                                }
                                else if(allTiles[tileCount].minesAround == 2){
                                    tile.setTexture(numberTwo);
                                }
                                else if(allTiles[tileCount].minesAround == 3){
                                    tile.setTexture(numberThree);
                                }
                                else if(allTiles[tileCount].minesAround == 4){
                                    tile.setTexture(numberFour);
                                }
                                else if(allTiles[tileCount].minesAround == 5){
                                    tile.setTexture(numberFive);
                                }
                                else if(allTiles[tileCount].minesAround == 6){
                                    tile.setTexture(numberSix);
                                }
                                else if(allTiles[tileCount].minesAround == 7){
                                    tile.setTexture(numberSeven);
                                }
                                else if(allTiles[tileCount].minesAround == 8){
                                    tile.setTexture(numberEight);
                                }
                                tile.setPosition(static_cast<float>((j * 32)), static_cast<float>(i * 32));
                                window.draw(tile);

                                // Sets the texture based on the mines around
                            }
                            // Logic if tile is not a mine but contains nearby mines and is revealed

                            if(allTiles[tileCount].minesAround == 0){
                                if(tleftValid(allTiles, tileCount, colCount)){
                                    int topLeft = tileCount - colCount - 1;
                                    if(!(allTiles[topLeft].isMine) && !(allTiles[topLeft].hasFlag)){
                                        allTiles[topLeft].isHidden = false;
                                    }
                                }
                                // Adds count to left-top square

                                if(topValid(allTiles, tileCount, colCount)){
                                    int topMiddle = tileCount - colCount;
                                    if(!(allTiles[topMiddle].isMine) && !(allTiles[topMiddle].hasFlag)){
                                        allTiles[topMiddle].isHidden = false;
                                    }
                                }
                                // Adds count to left-middle square

                                if(trightValid(allTiles, tileCount, colCount)){
                                    int topRight = tileCount - colCount + 1;
                                    if(!(allTiles[topRight].isMine) && !(allTiles[topRight].hasFlag)){
                                        allTiles[topRight].isHidden = false;
                                    }
                                }
                                // Adds count to left-right square

                                if(leftValid(allTiles, tileCount)){
                                    int middleLeft = tileCount - 1;
                                    if(!(allTiles[middleLeft].isMine) && !(allTiles[middleLeft].hasFlag)){
                                        allTiles[middleLeft].isHidden = false;
                                    }
                                }
                                // Adds count to middle-left square

                                if(rightValid(allTiles, tileCount, colCount)){
                                    int middleRight = tileCount + 1;
                                    if(!(allTiles[middleRight].isMine) && !(allTiles[middleRight].hasFlag)){
                                        allTiles[middleRight].isHidden = false;
                                    }
                                }
                                // Adds count to middle-right square

                                if(bleftValid(allTiles, tileCount, rowCount, colCount)){
                                    int bottomLeft = tileCount + colCount - 1;
                                    if(!(allTiles[bottomLeft].isMine) && !(allTiles[bottomLeft].hasFlag)){
                                        allTiles[bottomLeft].isHidden = false;
                                    }
                                }
                                // Adds count to bottom-top square

                                if(bottomValid(allTiles, tileCount, rowCount, colCount)){
                                    int bottomMiddle = tileCount + colCount;
                                    if(!(allTiles[bottomMiddle].isMine) && !(allTiles[bottomMiddle].hasFlag)){
                                        allTiles[bottomMiddle].isHidden = false;
                                    }
                                }
                                // Adds count to bottom-middle square

                                if(brightValid(allTiles, tileCount, rowCount, colCount)){
                                    int bottomRight = tileCount + colCount + 1;
                                    if(!(allTiles[bottomRight].isMine) && !(allTiles[bottomRight].hasFlag)){
                                        allTiles[bottomRight].isHidden = false;
                                    }
                                }
                                // Adds count to bottom-right square
                            }
                        }
                        // Logic if the tile is not hidden and not a mine
                    }
                }
            }
            // Displays the current state of all the tiles on the window

            window.draw(face);
            window.draw(debug);
            window.draw(status);
            window.draw(leaderboard);
            window.display();
        }
        // Logic for in/out of leaderboard and tile appearance
    }
}
// Logic for game and leaderboard window

bool GameWindow::topValid(Tile* tiles, int current, int colCount){
    int checkIndex = current - colCount;
    if(tiles[checkIndex].y != 0){
        return true;
    }
    return false;
}
// Finds status of middle-top tile

bool GameWindow::leftValid(Tile* tiles, int current){
    int checkIndex = current - 1;
    if((tiles[checkIndex].x = tiles[current].x - 1)){
        return true;
    }
    return false;
}
// Finds status of left-middle tile

bool GameWindow::rightValid(Tile* tiles, int current, int colCount){
    int checkIndex = current + 1;
    if((tiles[checkIndex].x = tiles[current].x + 1) && (tiles[checkIndex].x < (colCount + 1))){
        return true;
    }
    return false;
}
// Finds status of right-middle tile

bool GameWindow::bottomValid(Tile* tiles, int current, int rowCount, int colCount){
    int checkIndex = current + colCount;
    if((tiles[checkIndex].y < (rowCount + 1)) && checkIndex < (colCount * rowCount)){
        return true;
    }
    return false;
}
// Finds status of middle-bottom tile

bool GameWindow::tleftValid(Tile* tiles, int current, int colCount){
    int checkIndex = current - colCount - 1;
    if((tiles[checkIndex].y != 0) && (tiles[checkIndex].x = tiles[current].x - 1)){
        return true;
    }
    return false;
}
// Finds status of left-top tile

bool GameWindow::trightValid(Tile* tiles, int current, int colCount){
    int checkIndex = current - colCount + 1;
    if((tiles[checkIndex].y != 0) && (tiles[checkIndex].x = tiles[current].x + 1) && (tiles[checkIndex].x < (colCount + 1))){
        return true;
    }
    return false;
}
// Finds status of right-top tile

bool GameWindow::bleftValid(Tile* tiles, int current, int rowCount, int colCount){
    int checkIndex = current + colCount - 1;
    if((tiles[checkIndex].y < (rowCount + 1)) && (checkIndex < (colCount * rowCount)) && (tiles[checkIndex].x = tiles[current].x - 1)){
        return true;
    }
    return false;
}
// Finds status of bottom-left tile

bool GameWindow::brightValid(Tile* tiles, int current, int rowCount, int colCount){
    int checkIndex = current + colCount + 1;
    if((tiles[checkIndex].y < (rowCount + 1)) && (checkIndex < (colCount * rowCount)) && (tiles[checkIndex].x = tiles[current].x + 1) && (tiles[checkIndex].x < (colCount + 1))){
        return true;
    }
    return false;
}
// Finds status of bottom-right tile
