#include "WelcomeWindow.h"
#include "GameWindow.h"
#include "LeaderboardWindow.h"
using namespace std;
using namespace Algorithms;

int main() {
    int rowCount, colCount, mineCount;
    readConfig("config.cfg", &colCount, &rowCount, &mineCount);

    WelcomeWindow welcome_screen;
    welcome_screen.mainMenu(rowCount, colCount);

    if(!(welcome_screen.getForceClosed())){
        GameWindow game_screen(welcome_screen.getName(), welcome_screen.getUsername(), 0);
        game_screen.gameScreen(rowCount, colCount, mineCount);
    }

    return 0;
}
