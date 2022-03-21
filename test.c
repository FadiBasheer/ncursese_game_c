//
// Created by fadi on 2022-03-15.
//

#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#define WIDTH 100
#define HEIGHT 30

int startx = 0;
int starty = 0;

struct class_obj {
    char symbol;
    int x;
    int y;
    int direction;
};

struct application_settings {
    WINDOW *win;
    int X;
    int Y;
    struct class_obj ob;
};

void display_screen(WINDOW *win, int Y, int X, struct class_obj ob);

void *display_bullet(void *arg);

void *display_enemies(void *arg);

// Create objects
struct class_obj player = {};
struct class_obj bullet = {};
struct class_obj enemies[5] = {};

void obj_init(struct class_obj *obj, int x, int y, int dir, char obj_shape) {
    obj->x = x;
    obj->y = y;
    obj->direction = dir;
    obj->symbol = obj_shape;
//    strcpy(obj->symbol, objname);
}

void eneymies_init(struct class_obj obj[], int x, int y, int dir, char obj_shape) {
    for (int i = 0; i < 5; ++i) {
        obj[i].x = x;
        obj[i].y = y;
        obj[i].direction = dir;
        obj[i].symbol = obj_shape;
    }
}

int main() {
    WINDOW *win;
    int choice = 0;
    int c;
    int X = 1, Y = 1;
    pthread_t thread_1, thread_2;
    struct application_settings arg;

    obj_init(&player, 5, 5, 0, 'D');
    // Bullet
    obj_init(&bullet, player.x, player.x, 0, '*');

    eneymies_init(enemies, 50, 6, 0, 'O');

    initscr();
    clear();
    noecho();
    cbreak();    /* Line buffering disabled. pass on everything */
    startx = 0;
    starty = 0;


    win = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(win, TRUE);
    refresh();
    // display_screen(win, Y, X, player);

    if (pthread_create(&thread_1, NULL, &display_enemies, win) != 0) {
        perror("Failed to create thread");
    }


    while (1) {
        c = wgetch(win);
        //mvwdelch(win, Y, X);
        mvwaddch(win, Y, X, ' ');
        wrefresh(win);
        switch (c) {
            case KEY_UP:
                --Y;
                break;
            case KEY_DOWN:
                ++Y;
                break;
            case KEY_LEFT:
                --X;
                break;
            case KEY_RIGHT:
                ++X;
                break;
            case 's': {
                arg.win = win;
                arg.X = X;
                arg.Y = Y;
                arg.ob = bullet;
                if (pthread_create(&thread_2, NULL, &display_bullet, &arg) != 0) {
                    perror("Failed to create thread");
                }
                break;
            }
            default:
                refresh();
                break;
        }
        mvwaddch(win, Y, X, player.symbol);
        wrefresh(win);
    }
    clrtoeol();
    refresh();
    endwin();
    return 0;
}

void *display_bullet(void *arg) {
    struct application_settings *app_settings;
    app_settings = arg;

    struct timespec ts;
    uint16_t delay = 20;
    ts.tv_sec = 0;
    ts.tv_nsec = delay * 650000000;
    int y = app_settings->Y;
    int x = app_settings->X;
    for (int i = x + 2; i < 35; i++) {
        mvwaddch(app_settings->win, y, i, app_settings->ob.symbol);
        wrefresh(app_settings->win);
        nanosleep(&ts, NULL);
        mvwaddch(app_settings->win, y, i, ' ');
    }
    return 0;
}

int printRandoms(int lower, int upper) {
    return (rand() % (upper - lower + 1)) + lower;
}


void *display_enemies(void *arg) {
    WINDOW *win_en;
    win_en = arg;

    struct timespec ts;
    uint16_t delay = 20;
    ts.tv_sec = 0;
    ts.tv_nsec = delay * 20000000;

    while (1) {
        for (int i = 0; i < 5; ++i) {
            int rand = printRandoms(-1, 1);
            enemies[i].x--;
            enemies[i].y += rand;
            mvwaddch(win_en, enemies[i].y, enemies[i].x, enemies[i].symbol);
            wrefresh(win_en);
        }
        nanosleep(&ts, NULL);
        for (int i = 0; i < 5; ++i) {
            mvwaddch(win_en, enemies[i].y, enemies[i].x, ' ');
            //  mvwdelch(win_en, enemies[i].y, enemies[i].x);
            wrefresh(win_en);
        }
        // mvwdelch(win_en, enemies[i].y, enemies[i].x);
//        wclear(win_en);
//        wrefresh(win_en);
    }
}

//void display_screen(WINDOW *win, int Y, int X, struct class_obj ob) {
////    refresh();
////    clear();
//    //  mvwprintw(win, Y, X, ob.symbolz);
//
//    mvwaddch(win, Y, X, ob.symbol);
//    wrefresh(win);
//}