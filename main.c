//
// Created by fadi on 2022-03-03.
//

#include <ncurses.h>
#include <string.h>


int xLoc, yLoc, xMax, yMax;
WINDOW *cur_win;
char character[4] = {0};

void clearCharacterPath() {
    mvwprintw(cur_win, yLoc, xLoc, "    ");
}

void mv_up() {
    clearCharacterPath();
    yLoc--;
    strcpy(character, "/O\\");
    if (yLoc < 1) {
        yLoc = 1;
    }
}

void mv_down() {
    clearCharacterPath();
    yLoc++;
    strcpy(character, "\\0/");
    if (yLoc > (yMax - 2)) {
        yLoc = (yMax - 2);
    }
}

void mv_left() {
    clearCharacterPath();
    xLoc--;
    strcpy(character, "<0|");
    if (xLoc < 1) {
        xLoc = 1;
    }
}

void mv_right() {
    clearCharacterPath();
    xLoc++;
    strcpy(character, "|0>");
    if (xLoc > (xMax - 2)) {
        xLoc = (xMax - 2);
    }
}

int getMove() {
    int choice = wgetch(cur_win);
    switch (choice) {
        case KEY_UP:
            mv_up();
            break;
        case KEY_DOWN:
            mv_down();
            break;
        case KEY_LEFT:
            mv_left();
            break;
        case KEY_RIGHT:
            mv_right();
            break;
        default:
            break;
    }
    return choice;
}

void display() {
    mvwprintw(cur_win, yLoc, xLoc, character);
}

int main() {
    initscr();
    noecho();
    cbreak();

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *playerWin = newwin(yMax / 2 + 5, xMax - 2, 2, 2);
    box(playerWin, 0, 0);
    curs_set(FALSE); // hide cursor

    refresh();
    wrefresh(playerWin);

    // Player(playerWin, 1, 1, "team1");
    cur_win = playerWin;
    yLoc = 1;
    xLoc = 1;
    //getmaxyx(playerWin, yMax, xMax);
    keypad(cur_win, true);
    strcpy(character, "/O\\");
    display();


    while (getMove() != 'q') {
        display();
        wrefresh(playerWin);
    }

    delwin(playerWin);
    endwin();
    return 0;
}

//
//// Created by fadi on 2022-03-03.
////
//
//#include "Player.h"
//
//#include <utility>
//#include <cstring>
//#include <iostream>
//
//Player::Player(WINDOW *win, int y, int x, string team) {
//    cur_win = win;
//    yLoc = y;
//    xLoc = x;
//    getmaxyx(win, yMax, xMax);
//    keypad(cur_win, true);
//    this->team = std::move(team);
//    strcpy(character, "/O\\");
//}
//
//void Player::mv_up() {
//    clearCharacterPath();
//    yLoc--;
//    strcpy(character, "/O\\");
//    if (yLoc < 1) {
//        yLoc = 1;
//    }
//    cout << yLoc << " " << xLoc;
//}
//
//void Player::mv_down() {
//    clearCharacterPath();
//    yLoc++;
//    strcpy(character, "\\0/");
//    if (yLoc > (yMax - 2)) {
//        yLoc = (yMax - 2);
//    }
//}
//
//void Player::mv_left() {
//    clearCharacterPath();
//    xLoc--;
//    strcpy(character, "<0|");
//    if (xLoc < 1) {
//        xLoc = 1;
//    }
//}
//
//void Player::mv_right() {
//    clearCharacterPath();
//    xLoc++;
//    strcpy(character, "|0>");
//    if (xLoc > (xMax - 2)) {
//        xLoc = (xMax - 2);
//    }
//}
//
//int Player::getMove() {
//    int choice = wgetch(cur_win);
//    switch (choice) {
//        case KEY_UP:
//            mv_up();
//            break;
//        case KEY_DOWN:
//            mv_down();
//            break;
//        case KEY_LEFT:
//            mv_left();
//            break;
//        case KEY_RIGHT:
//            mv_right();
//            break;
//        default:
//            break;
//    }
//    return choice;
//}
//
//void Player::display() {
//    mvwprintw(cur_win, yLoc, xLoc, character);
//}
//
//void Player::clearCharacterPath() {
//    mvwprintw(cur_win, yLoc, xLoc, "    ");
//}