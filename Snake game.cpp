#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 40
#define HEIGHT 20
#define MAX_SNAKE_LENGTH 800

int gameOver;
int score;

int snakeX[MAX_SNAKE_LENGTH];
int snakeY[MAX_SNAKE_LENGTH];
int snakeLength;

int foodX;
int foodY;

enum Direction {
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum Direction direction;

void setup(void);
void draw(void);
void input(void);
void logic(void);
void generateFood(void);
void hideCursor(void);
void moveCursor(int x, int y);

int main(void) {
    setup();

    while (!gameOver) {
        draw();
        input();
        logic();
        Sleep(100);
    }

    moveCursor(0, HEIGHT + 4);

    printf("\n=================================\n");
    printf("            GAME OVER\n");
    printf("=================================\n");
    printf("Your Final Score: %d\n", score);
    printf("=================================\n");

    return 0;
}

void setup(void) {
    gameOver = 0;
    score = 0;

    snakeLength = 3;

    snakeX[0] = WIDTH / 2;
    snakeY[0] = HEIGHT / 2;

    snakeX[1] = snakeX[0] - 1;
    snakeY[1] = snakeY[0];

    snakeX[2] = snakeX[0] - 2;
    snakeY[2] = snakeY[0];

    direction = RIGHT;

    srand((unsigned int)time(NULL));

    generateFood();
    hideCursor();

    system("cls");
}

void draw(void) {
    int x;
    int y;
    int i;
    int printedSnake;

    moveCursor(0, 0);

    printf("============== SNAKE GAME ==============\n");
    printf("Score: %d\n", score);
    printf("Controls: W = Up, S = Down, A = Left, D = Right, X = Exit\n\n");

    for (x = 0; x < WIDTH + 2; x++) {
        printf("#");
    }

    printf("\n");

    for (y = 0; y < HEIGHT; y++) {
        printf("#");

        for (x = 0; x < WIDTH; x++) {
            printedSnake = 0;

            if (x == snakeX[0] && y == snakeY[0]) {
                printf("O");
                printedSnake = 1;
            } else {
                for (i = 1; i < snakeLength; i++) {
                    if (x == snakeX[i] && y == snakeY[i]) {
                        printf("o");
                        printedSnake = 1;
                        break;
                    }
                }
            }

            if (!printedSnake) {
                if (x == foodX && y == foodY) {
                    printf("*");
                } else {
                    printf(" ");
                }
            }
        }

        printf("#\n");
    }

    for (x = 0; x < WIDTH + 2; x++) {
        printf("#");
    }

    printf("\n");
}

void input(void) {
    char key;

    if (_kbhit()) {
        key = (char)_getch();

        switch (key) {
            case 'a':
            case 'A':
                if (direction != RIGHT) {
                    direction = LEFT;
                }
                break;

            case 'd':
            case 'D':
                if (direction != LEFT) {
                    direction = RIGHT;
                }
                break;

            case 'w':
            case 'W':
                if (direction != DOWN) {
                    direction = UP;
                }
                break;

            case 's':
            case 'S':
                if (direction != UP) {
                    direction = DOWN;
                }
                break;

            case 'x':
            case 'X':
                gameOver = 1;
                break;
        }
    }
}

void logic(void) {
    int i;
    int previousX;
    int previousY;
    int temporaryX;
    int temporaryY;

    previousX = snakeX[0];
    previousY = snakeY[0];

    switch (direction) {
        case LEFT:
            snakeX[0]--;
            break;

        case RIGHT:
            snakeX[0]++;
            break;

        case UP:
            snakeY[0]--;
            break;

        case DOWN:
            snakeY[0]++;
            break;

        default:
            break;
    }

    for (i = 1; i < snakeLength; i++) {
        temporaryX = snakeX[i];
        temporaryY = snakeY[i];

        snakeX[i] = previousX;
        snakeY[i] = previousY;

        previousX = temporaryX;
        previousY = temporaryY;
    }

    if (snakeX[0] < 0 ||
        snakeX[0] >= WIDTH ||
        snakeY[0] < 0 ||
        snakeY[0] >= HEIGHT) {
        gameOver = 1;
    }

    for (i = 1; i < snakeLength; i++) {
        if (snakeX[0] == snakeX[i] &&
            snakeY[0] == snakeY[i]) {
            gameOver = 1;
        }
    }

    if (snakeX[0] == foodX && snakeY[0] == foodY) {
        score += 10;

        if (snakeLength < MAX_SNAKE_LENGTH) {
            snakeX[snakeLength] = snakeX[snakeLength - 1];
            snakeY[snakeLength] = snakeY[snakeLength - 1];
            snakeLength++;
        }

        generateFood();
    }
}

void generateFood(void) {
    int validPosition;
    int i;

    do {
        validPosition = 1;

        foodX = rand() % WIDTH;
        foodY = rand() % HEIGHT;

        for (i = 0; i < snakeLength; i++) {
            if (foodX == snakeX[i] && foodY == snakeY[i]) {
                validPosition = 0;
                break;
            }
        }
    } while (!validPosition);
}

void hideCursor(void) {
    HANDLE consoleHandle;
    CONSOLE_CURSOR_INFO cursorInfo;

    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleCursorInfo(consoleHandle, &cursorInfo);

    cursorInfo.bVisible = FALSE;

    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void moveCursor(int x, int y) {
    HANDLE consoleHandle;
    COORD position;

    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    position.X = (SHORT)x;
    position.Y = (SHORT)y;

    SetConsoleCursorPosition(consoleHandle, position);
}