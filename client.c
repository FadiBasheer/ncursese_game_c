
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

void display_screen(WINDOW *win, int X, int Y, int sockfd, struct sockaddr_in servaddr);

int open_joystick(char *device_name) {
    int fd = -1;

    if (device_name == NULL) {
        return fd;
    }

    fd = open(device_name, O_RDONLY | O_NONBLOCK);

    if (fd < 0) {
        printf("Could not locate joystick device %s\n", device_name);
        exit(1);
    }
    return fd;
}


//void process_event(struct js_event jse) {
////    printf("number: %d\n", jse.number);
////    printf("value: %d\n", jse.value);
////    printf("type: %d\n", jse.type);
//
//    if (jse.type == 2) {
//        if (jse.number == 7) {
//            if (jse.value < 0) {
//                printf("UP\n");
//            } else if (jse.value > 0) {
//                printf("Down\n");
//            }
//        } else if (jse.number == 6) {
//            if (jse.value < 0) {
//                printf("Left\n");
//            } else if (jse.value > 0) {
//                printf("Right\n");
//            }
//        }
//    }
//
//    if (jse.type == 1 && jse.value > 0) {
//        printf("%d\n", jse.number);
//    }
//
//}


int main() {
    int fd;
    struct js_event jse;

    fd = open_joystick("/dev/input/js1");

    //////////////////////////////////////////////////////////////////////
    WINDOW *win;
    int X = 1, Y = 1;
    int choice = 0;
    int c;

    int sockfd;
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;


    initscr();
    clear();
    noecho();
    cbreak();    /* Line buffering disabled. pass on everything */
    startx = 0;
    starty = 0;
    win = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(win, TRUE);
    box(win, 0, 0);
    refresh();
    display_screen(win, X, Y, sockfd, servaddr);

//    while (1) {
//        c = wgetch(win);
//        switch (c) {
//            case KEY_UP:
//                --X;
//                break;
//            case KEY_DOWN:
//                ++X;
//                break;
//            case KEY_LEFT:
//                --Y;
//                break;
//            case KEY_RIGHT:
//                ++Y;
//                break;
//            case 10:
//                choice = X;
//                break;
//        }

    while (1) {
        while (read(fd, &jse, sizeof(jse)) > 0) {
            if (jse.type == 2) {
                if (jse.number == 7) {
                    if (jse.value < 0) {
                        // printf("UP\n");
                        --X;
                    } else if (jse.value > 0) {
                        //printf("Down\n");
                        ++X;
                    }
                } else if (jse.number == 6) {
                    if (jse.value < 0) {
                        // printf("Left\n");
                        --Y;
                    } else if (jse.value > 0) {
                        //printf("Right\n");
                        ++Y;
                    }
                }
            }

            if (jse.type == 1 && jse.value > 0) {
                printf("%d\n", jse.number);
            }

            display_screen(win, X, Y, sockfd, servaddr);
//        if (choice != 0)    /* User did a choice come out of the infinite loop */
//            break;
        }
    }
    clrtoeol();
    refresh();
    endwin();

    close(sockfd);
    return 0;
}

int32_t buffer[3];

void display_screen(WINDOW *win, int X, int Y, int sockfd, struct sockaddr_in servaddr) {

    buffer[0] = X;
    buffer[1] = Y;

    mvwaddch(win, X, Y, ' ');
    wrefresh(win);
    sendto(sockfd, buffer, sizeof(buffer),
           MSG_CONFIRM, (const struct sockaddr *) &servaddr,
           sizeof(servaddr));
}