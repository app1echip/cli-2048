#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h> //for color switch

#define CONTINUE 0
#define WIN 1
#define LOSE 2
#define NEWGAME 3
//define game status

typedef struct
{
    int tile[4][4];
    int score;
} playground;

void newTile(int ar[][4], int n)
{
    srand((unsigned)time(NULL));
    while (n > 0)
    {
        int x = rand() % 4, y = rand() % 4;
        //pick vertical and horizontal axis at random
        if (ar[x][y] == 0)
        {
            ar[x][y] = rand() % 2 * 2 + 2;
            --n;
            //fill in the tile if empty
        }
    }
}
void drawBoard(playground *p, int *h)
{
    system("cls");

    if ((p->score) > *h)
        *h = p->score;
    //get high score

    printf("HIGH%17d\n"
           "SCORE%16d\n"
           "+----+----+----+----+\n",
           *h, p->score);
    //print high and score
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            putchar('|');
            if (p->tile[i][j] == 0)
                printf("    ");
            //print blank space if 0
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), log(p->tile[i][j]) / log(2));
                //change font color by the index
                printf("%4d", p->tile[i][j]);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                //restore color
            }
        }
        printf("|\n"
               "+----+----+----+----+\n");
    }
}
void swipe(int ar[][4], int x, int y)
{
    for (int i = 0; i < 4; ++i)
        for (int j = (1 + x + y) * 3 / 2; j != (1 - x - y) * 3 / 2; j -= x + y)
            for (int k = j - x; k > -1 && k < 4; k -= x + y)
            {
                int m = i * x * x + j * y * y, n = i + j - m;
                int a = i * x * x + k * y * y, b = i + k - a;
                //switch i and j, i and k, in math way
                if (ar[m][n] == 0)
                {
                    ar[m][n] = ar[a][b];
                    ar[a][b] = 0;
                    //move tile to non-empty space
                }
            }
}
void merge(playground *p, int x, int y)
{
    for (int i = 0; i < 4; ++i)
        for (int j = (1 + x + y) * 3 / 2; j != (1 - x - y) * 3 / 2; j -= x + y)
        {
            int m = i * x * x + j * y * y, n = i + j - m;
            if (p->tile[m][n] == p->tile[m - y][n - x])
            {
                p->score += p->tile[m][n] *= 2;
                //join 2 same tiles and replace the former, scoring btw
                p->tile[m - y][n - x] = 0;
                //set the latter to 0
            }
        }
}
int WinLose(int ar[][4], int g)
{
    int r = LOSE;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
        {
            if (ar[i][j] == g)
                return WIN;
            //win when goal found
            if (ar[i][j] == 0)
                r = CONTINUE;
            //find 0 btw
        }
    if (r == CONTINUE)
        return CONTINUE;
    //continue if 0 exit

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 3; ++j)
            if (ar[i][j] == ar[i][j + 1] || ar[j][i] == ar[j + 1][i])
                return CONTINUE;
    //continue if mergable
    return LOSE;
    //lose if no previous condition matched
}
void helppage()
{
    printf("HOW TO PLAY:\n"
           "[W/S/A/D] Move\n"
           "[U]ndo  [R]estart\n"
           "[Q]uit  [H]elp\n"
           "[G]oal\n");
}