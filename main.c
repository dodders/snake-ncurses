#include <stdio.h>
#include <ncurses.h>

#define WIDTH 50
#define HEIGHT 20

#define MAXRATE 250
#define MINRATE 100
#define LENGTH 40

enum direction { UP, DOWN, RIGHT, LEFT };

typedef struct spart {
    int x;
    int y;
} snakepart;

snakepart snake[LENGTH];

int move_snake(WINDOW* win, int direction, snakepart snake[]);

int main(int argc, char** argv) {
    puts("starting...");

    WINDOW *world;
    int offsetx, offsety, ch;

    initscr();
    noecho();
    cbreak();
    timeout(MAXRATE);
    keypad(stdscr, TRUE);
    printw("Snake v 1.0 - press x to quit...");

    refresh();

    offsetx = (COLS - WIDTH) / 2;
    offsety = (LINES - HEIGHT) / 2;

    world = newwin(HEIGHT, WIDTH, offsety, offsetx);

    int sbegx = (WIDTH - LENGTH) / 2;
    int sbegy = (HEIGHT - 1) / 2;

    for (int i = 0; i < LENGTH; i++) {
        snake[i].x = sbegx + i;
        snake[i].y = sbegy;
    }

    int cur_dir = RIGHT;
    int newrate = MAXRATE;
    int reps = 0;

    while ((ch = getch()) != 'x') {
        reps++;
        move_snake(world, cur_dir, snake);
        if (ch != ERR) {
            switch(ch) {
                case KEY_UP:
                    cur_dir = UP; break;
                case KEY_DOWN:
                    cur_dir =  DOWN; break;
                case KEY_LEFT:
                    cur_dir =  LEFT; break;
                case KEY_RIGHT:
                    cur_dir =  RIGHT; break;
                default:
                    break;
            }
        }
        if (reps > 10) {
            if ((newrate - 10) > MINRATE) {
                newrate = newrate - 10;
            }
            timeout(newrate);
            mvprintw(2,0, "Refresh rate is %.3d", newrate);
            reps = 0;
        }
    }

    box(world, 0, 0);
    wrefresh(world);
    getch();
    delwin(world);
    endwin();

    return 0;
}

int move_snake(WINDOW* win, int direction, snakepart snake[]) {

        wclear(win);
        for (int i = 0; i < LENGTH - 1; i++) {
            snake[i] = snake[i + 1];
            mvwaddch(win, snake[i].y, snake[i].x, '#');
        }

        int x = snake[LENGTH - 1].x;
        int y = snake[LENGTH - 1].y;
        switch (direction) {
            case UP:
                y - 1 == 0 ? y = HEIGHT - 2 : y--; break;
            case DOWN:
                y + 1 == HEIGHT - 1 ? y = 1 : y++; break;
            case RIGHT:
                x + 1 == WIDTH - 1 ? x = 1 : x++; break;
            case LEFT:
                x - 1 == 0 ? x = WIDTH - 2 : x--; break;
            default:
                break;
        }

        snake[LENGTH - 1].x = x;
        snake[LENGTH - 1].y = y;
        mvwaddch(win, y, x, '#');
        box(win, 0, 0);
        wrefresh(win);
        return 0;
}

















