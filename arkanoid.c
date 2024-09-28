/*
    Arkanoid is a classic arcade-style game where the objective is to complete levels by destroying targets with a bouncing ball.
    Copyright (C) 2024  Deezer13

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include "headers/kbhit.h"
#include "headers/fillerCoordinatesMalloc.h"
#include "headers/fillerCoordinatesFree.h"

#define M_PI 3.14159265358979323846
#define WIDTH 66
#define HEIGHT 25

typedef struct
{
    unsigned short x, y, width;
} TRacket;

typedef struct
{
    float x, y, alpha, speed;
    unsigned short rx, ry;
} TBall;

void initRacket(TRacket *);
void putRacket(const TRacket *, char[HEIGHT][WIDTH + 1]);
void moveRacket(short, TRacket *);
void moveBall(float, float, TBall *);
void initBall(TBall *);
void putBall(TBall *, char[HEIGHT][WIDTH + 1]);
void runBall(unsigned int *, unsigned short *, unsigned short *, TBall *, char[HEIGHT][WIDTH + 1], unsigned short ***);
void initField(unsigned short *, char[HEIGHT][WIDTH + 1]);
void show(const unsigned int *, const unsigned int *, const unsigned int *, char[HEIGHT][WIDTH + 1]);
void start(unsigned int *, unsigned int *, unsigned short *, unsigned int *, TBall *, TRacket *, char[HEIGHT][WIDTH + 1]);
void filler(unsigned short, unsigned short, unsigned short *, char[HEIGHT][WIDTH + 1], unsigned short **);
void showLevel(unsigned short *, unsigned short *, char[HEIGHT][WIDTH + 1], unsigned short **);

int main()
{
    srand(time(0));

    TRacket racket;
    TBall ball;

    char field[HEIGHT][WIDTH + 1];

    unsigned short level = 1, sizeFillerCoordinates = 13;
    unsigned int hitCnt = 0, hitMaxCnt = 0, fallCnt = 0;

    unsigned short **fillerCoordinates = fillerCoordinatesMalloc(&sizeFillerCoordinates);

    start(&hitCnt, &hitMaxCnt, &level, &fallCnt, &ball, &racket, field);

    char key;
    bool run = false;
    while (1)
    {
        if (kbhit())
        {
            key = getchar();
            switch (key)
            {
            case 'q':
                printf("\n");
                fillerCoordinatesFree(fillerCoordinates);
                exit(EXIT_SUCCESS);
                break;
            case 'w':
                run = true;
                break;
            case 'a':
                moveRacket(racket.x - 2, &racket);
                break;
            case 'd':
                moveRacket(racket.x + 2, &racket);
                break;
            default:
                continue;
            }
        }
        printf("\e[1;1H\e[2J");
        initField(&level, field);
        showLevel(&level, &sizeFillerCoordinates, field, fillerCoordinates);
        putRacket(&racket, field);
        run ? runBall(&hitCnt, &level, &sizeFillerCoordinates, &ball, field, &fillerCoordinates) : moveBall(racket.x + racket.width / 2, racket.y - 1, &ball);
        if (ball.ry >= HEIGHT)
        {
            if (hitCnt > hitMaxCnt)
                hitMaxCnt = hitCnt;
            hitCnt = 0;
            fallCnt++;
            start(&hitCnt, &hitMaxCnt, &level, &fallCnt, &ball, &racket, field);
            run = false;
        }
        putBall(&ball, field);
        show(&hitCnt, &hitMaxCnt, &fallCnt, field);
        usleep(10000);
    }

    return 0;
}

void initRacket(TRacket *racket)
{
    racket->width = 7;
    racket->x = (WIDTH - racket->width) / 2;
    racket->y = HEIGHT - 1;
}

void putRacket(const TRacket *racket, char field[HEIGHT][WIDTH + 1])
{
    for (unsigned short i = racket->x; i < racket->x + racket->width; i++)
        field[racket->y][i] = '=';
}

void moveRacket(short x, TRacket *racket)
{
    racket->x = x < 1 ? 1 : (x + racket->width > WIDTH - 2 ? WIDTH - 2 - racket->width : x);
}

void moveBall(float x, float y, TBall *ball)
{
    ball->x = x;
    ball->y = y;
    ball->rx = (unsigned short)round(ball->x);
    ball->ry = (unsigned short)round(ball->y);
}

void initBall(TBall *ball)
{
    moveBall(2, 2, ball);
    ball->alpha = -1;
    ball->speed = 0.4f;
}

void putBall(TBall *ball, char field[HEIGHT][WIDTH + 1])
{
    field[ball->ry][ball->rx] = '*';
}

void runBall(unsigned int *hitCnt, unsigned short *level, unsigned short *sizeFillerCoordinates, TBall *ball, char field[HEIGHT][WIDTH + 1], unsigned short ***fillerCoordinates)
{
    if (ball->alpha < 0)
        ball->alpha += M_PI * 2;
    else if (ball->alpha > M_PI * 2)
        ball->alpha -= M_PI * 2;

    TBall preBall = *ball;
    moveBall(ball->x + cos(ball->alpha) * ball->speed, ball->y + sin(ball->alpha) * ball->speed, ball);

    if (field[ball->ry][ball->rx] == '#' || field[ball->ry][ball->rx] == '=')
    {
        preBall.speed += 0.000005f + ((float)rand() / RAND_MAX) * (0.00005f - 0.000005f);

        if (field[ball->ry][ball->rx] == '=')
            (*hitCnt)++;

        if (ball->rx != preBall.rx && ball->ry != preBall.ry)
        {
            // if (field[preBall.ry][ball->rx] == field[ball->ry][preBall.rx])
            //     preBall.alpha += M_PI;
            // else
            //{
            //     if (field[preBall.ry][ball->rx])
            //         preBall.alpha = (2 * M_PI - preBall.alpha) + M_PI;
            //     else
            preBall.alpha = (2 * M_PI - preBall.alpha);
            //}
        }
        else if (ball->ry == preBall.ry)
        {
            preBall.alpha = (2 * M_PI - preBall.alpha) + M_PI;
        }
        else
        {
            preBall.alpha = (2 * M_PI - preBall.alpha);
        }

        moveBall(preBall.rx, preBall.ry, ball);
        *ball = preBall;
        runBall(hitCnt, level, sizeFillerCoordinates, ball, field, fillerCoordinates);
    }
    else if (field[preBall.ry][preBall.rx] == '+')
    {
        static unsigned short count = 0;

        field[preBall.ry][preBall.rx] = ' ';
        (*fillerCoordinates)[count][0] = preBall.ry;
        (*fillerCoordinates)[count][1] = preBall.rx;
        if (++count == *sizeFillerCoordinates - 1)
        {
            if (*level == 3)
            {
                fillerCoordinatesFree(*fillerCoordinates);
                printf("GAME OVER!\n");
                exit(EXIT_SUCCESS);
            }

            count = 0;

            if (++(*level) == 2)
                *sizeFillerCoordinates = 17;
            else if (*level == 3)
                *sizeFillerCoordinates = 385;

            fillerCoordinatesFree(*fillerCoordinates);
            *fillerCoordinates = fillerCoordinatesMalloc(sizeFillerCoordinates);
        }
    }
}

void initField(unsigned short *level, char field[HEIGHT][WIDTH + 1])
{
    unsigned short i;

    for (i = 0; i < WIDTH - 1; i++)
        field[0][i] = '#';
    field[0][WIDTH] = '\0';

    strncpy(field[1], field[0], WIDTH - 1);
    for (i = 1; i < WIDTH - 2; i++)
        field[1][i] = ' ';

    for (i = 2; i < HEIGHT; i++)
        strncpy(field[i], field[1], WIDTH);

    if (*level == 2)
        for (i = 18; i < WIDTH - 18; i++)
            field[10][i] = '#';
    else if (*level == 3)
    {
        for (i = 15; i < WIDTH - 15; i++)
            field[1][i] = '#';
        for (i = 2; i < 14; i++)
        {
            field[i][15] = '#';
            field[i][WIDTH - 16] = '#';
        }
    }
}

void show(const unsigned int *hitCnt, const unsigned int *hitMaxCnt, const unsigned int *fallCnt, char field[HEIGHT][WIDTH + 1])
{
    for (unsigned short i = 0; i < HEIGHT; i++)
        printf("%s\n", field[i]);
    printf("\t\t\t\t\t\t\t\t\tHit: %u\t\tMax: %u\t\tFall Count: %u\n", *hitCnt, *hitMaxCnt, *fallCnt);
    fflush(stdout);
}

void start(unsigned int *hitCnt, unsigned int *hitMaxCnt, unsigned short *level, unsigned int *fallCnt, TBall *ball, TRacket *racket, char field[HEIGHT][WIDTH + 1])
{
    system("clear");
    initField(level, field);
    initRacket(racket);
    initBall(ball);
    putRacket(racket, field);
    putBall(ball, field);
    show(hitCnt, hitMaxCnt, fallCnt, field);
}

void filler(unsigned short x, unsigned short y, unsigned short *sizeFillerCoordinates, char field[HEIGHT][WIDTH + 1], unsigned short **fillerCoordinates)
{
    char occupied[4] = {'1', '1', '1', '1'};
    for (unsigned short i = 0; i < *sizeFillerCoordinates; i++)
    {
        if (fillerCoordinates[i][0] == 0)
            continue;

        if (fillerCoordinates[i][0] == y && fillerCoordinates[i][1] == x)
            occupied[0] = '0';
        if (fillerCoordinates[i][0] == y && fillerCoordinates[i][1] == x + 1)
            occupied[1] = '0';
        if (fillerCoordinates[i][0] == y + 1 && fillerCoordinates[i][1] == x)
            occupied[2] = '0';
        if (fillerCoordinates[i][0] == y + 1 && fillerCoordinates[i][1] == x + 1)
            occupied[3] = '0';
    }

    if (occupied[0] != '0')
        field[y][x] = '+';
    if (occupied[1] != '0')
        field[y][x + 1] = '+';
    if (occupied[2] != '0')
        field[y + 1][x] = '+';
    if (occupied[3] != '0')
        field[y + 1][x + 1] = '+';
}

void showLevel(unsigned short *level, unsigned short *sizeFillerCoordinates, char field[HEIGHT][WIDTH + 1], unsigned short **fillerCoordinates)
{
    unsigned short i;
    switch (*level)
    {
    case 1:
        for (i = 0; i < 3; i++)
            filler((i == 0 ? WIDTH / 2 - 4 : (i == 1 ? WIDTH / 2 + 2 : WIDTH / 2 - 1)), 1, sizeFillerCoordinates, field, fillerCoordinates);
        break;
    case 2:
        for (i = 0; i < 4; i++)
        {
            unsigned short x = (i == 0) ? 7 : (i == 1) ? WIDTH - 9
                                          : (i == 2)   ? 32
                                                       : 32;
            unsigned short y = (i == 0 || i == 1) ? 10 : (i == 2) ? 7
                                                                  : 12;

            filler(x, y, sizeFillerCoordinates, field, fillerCoordinates);
        }
        break;
    case 3:
        for (i = 0; i < 12; i += 2)
        {
            for (unsigned short j = 0; j < 32; j += 2)
                filler(j + 17, i + 2, sizeFillerCoordinates, field, fillerCoordinates);
        }
        break;
    }
}
