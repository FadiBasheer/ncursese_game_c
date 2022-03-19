//
// Created by fadi on 2022-03-14.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <linux/joystick.h>
#include <fcntl.h>


#define PORT     8080
#define MAXLINE 1024

#define WIDTH 100
#define HEIGHT 30

int startx = 0;
int starty = 0;

struct class_obj {
    char symbol[20];
    int x;
    int y;
    int direction;
};


void display_screen(WINDOW *win, int X, int Y, struct class_obj ob);

void display_bullet(WINDOW *win, int X, int Y, struct class_obj ob);

//int open_joystick(char *device_name) {
//    int fd = -1;
//
//    if (device_name == NULL) {
//        return fd;
//    }
//
//    fd = open(device_name, O_RDONLY | O_NONBLOCK);
//
//    if (fd < 0) {
//        printf("Could not locate joystick device %s\n", device_name);
//        exit(1);
//    }
//    return fd;
//}

// Create objects
struct class_obj player = {};
struct class_obj bullet = {};
struct class_obj enemy[5] = {};

void obj_init(struct class_obj *obj, int x, int y, int dir, char *objname) {
    obj->x = x;
    obj->y = y;
    obj->direction = dir;
    strcpy(obj->symbol, objname);
}


int main() {
    int fd;
    struct js_event jse;

    //fd = open_joystick("/dev/input/js0");

    WINDOW *win;
    int X = 1, Y = 1;
    int highlight = 1;
    int choice = 0;
    int c;

//    obj_init(&player, 5, 5, 0, "|O|");
//    // Bullet
//    obj_init(&bullet, player.x, player.x, 0, "*");

//    int sockfd;
//    struct sockaddr_in servaddr;

    // Creating socket file descriptor
//    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
//        perror("socket creation failed");
//        exit(EXIT_FAILURE);
//    }

//    memset(&servaddr, 0, sizeof(servaddr));
//
//    // Filling server information
//    servaddr.sin_family = AF_INET;
//    servaddr.sin_port = htons(PORT);
//    servaddr.sin_addr.s_addr = INADDR_ANY;

    initscr();
    clear();
    noecho();
    cbreak();    /* Line buffering disabled. pass on everything */
    startx = (80 - WIDTH) / 2;
    starty = (24 - HEIGHT) / 2;

    win = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(win, TRUE);
    refresh();
    display_screen(win, X, Y, player);

    while (1) {
        c = wgetch(win);
        switch (c) {
            case KEY_UP:
                --X;
                break;
            case KEY_DOWN:
                ++X;
                break;
            case KEY_LEFT:
                --Y;
                break;
            case KEY_RIGHT:
                ++Y;
                break;
            default:
                refresh();
                break;
        }
        display_screen(win, X, Y, player);
        if (choice != 0)    /* User did a choice come out of the infinite loop */
            break;
    }

    //    while (1) {
//        while (read(fd, &jse, sizeof(jse)) > 0) {
//            if (jse.type == 2) {
//                if (jse.number == 7) {
//                    if (jse.value < 0) {
//                        // printf("UP\n");
//                        --X;
//                    } else if (jse.value > 0) {
//                        //printf("Down\n");
//                        ++X;
//                    }
//                } else if (jse.number == 6) {
//                    if (jse.value < 0) {
//                        // printf("Left\n");
//                        --Y;
//                    } else if (jse.value > 0) {
//                        //printf("Right\n");
//                        ++Y;
//                    }
//                }
//            }
//
//            if (jse.number == 0 && jse.type == 1) {
//                display_bullet(win, X, Y, bullet);
//            }
//            display_screen(win, X, Y, player);
//        }
//    }

    clrtoeol();
    refresh();
    endwin();
    return 0;
}


void display_bullet(WINDOW *win, int X, int Y, struct class_obj ob) {
    for (; X < 100;) {
//        refresh();
//        clear();
        X++;
        sleep(1);
        mvwprintw(win, Y, X, ob.symbol);
        // mvwaddch(win, X, Y, ob.symbol);
        wrefresh(win);
    }
}

void display_screen(WINDOW *win, int X, int Y, struct class_obj ob) {
    refresh();
    clear();
    mvwaddch(win, X, Y, '*');
    ///  mvwprintw(win, Y, X, ob.symbol);
    wrefresh(win);
}