/*
 * name : 2048.c
 * author : Fang Haonan
 * e-mail: yougoon@live.cn
 * date: 2018/12/21
 * description: simple 2048 game for Windows
 */

#include "2048.h"

playground board, board_PREV, board_TEMP;
int high = 0;

int main()
{
    int goal = 2048;

    int status = NEWGAME;
    do
    {
        FILE *fp;
        if ((fp = fopen("savedgame", "rb")) != NULL)
        {
            fread(&board, sizeof(board), 1, fp);
            fread(&board_PREV, sizeof(board_PREV), 1, fp);
            fclose(fp);
            remove("savedgame");
        }
        else
        {
            memset(&board, 0, sizeof(board));
            newTile(board.tile, 2);
            memcpy(&board_PREV, &board, sizeof(board));
        }
        //if previous game found, it will load automatically
        //else start a new round

        drawBoard(&board, &high);

        if (high == 0)
            helppage();
        //help page at first run(no high score found)

        status = CONTINUE;
        do
        {
            int x = 0, y = 0;
            switch (getch())
            {
            case 'w':
            case 'W':
                y = -1;
                break;
            case 's':
            case 'S':
                y = 1;
                break;
            case 'a':
            case 'A':
                x = -1;
                break;
            case 'd':
            case 'D':
                x = 1;
                break; //(x,y) used to move at desired direction
            case 'h':
            case 'H':
                helppage();
                continue;
            case 'u':
            case 'U':
                memcpy(&board, &board_PREV, sizeof(board));
                drawBoard(&board, &high);
                continue; //undo option
            case 'r':
            case 'R':
                status = NEWGAME;
                remove("savedgame");
                continue;
            case 'q':
            case 'Q':
                if (board.score != 0)
                {
                    fp = fopen("savedgame", "wb");
                    fwrite(&board, sizeof(board), 1, fp);
                    fwrite(&board_PREV, sizeof(board_PREV), 1, fp);
                    fclose(fp);
                }
                return 0; //save and quit
            case 'g':
            case 'G':
                printf("Enter your goal:");
                scanf("%d", &goal);
                printf("Goal changed to %d.\n", goal);
                continue; //change game goal
            default:
                continue;
            }

            memcpy(&board_TEMP, &board, sizeof(board));
            //backup board with board_TEMP and board_PREV(see below)

            swipe(board.tile, x, y);
            merge(&board, x, y);
            swipe(board.tile, x, y);
            //swipe and merge

            if (memcmp(&board_TEMP, &board, sizeof(board)) != 0)
            {
                newTile(board.tile, 1);
                memcpy(&board_PREV, &board_TEMP, sizeof(board));
                //backup board for undo function
            }
            else
                continue;
            //if any tile changes, a move is seen as valid
            //else no tile will be generated

            drawBoard(&board, &high);

            status = WinLose(board.tile, goal);
            //game status: win, lose or continue

            switch (status)
            {
            case LOSE:
                printf("      Game Over\n\n"
                       "[U]ndo, [R]estart or Quit\n");
                switch (getch())
                {
                case 'u':
                case 'U':
                    memcpy(&board, &board_PREV, sizeof(board));
                    drawBoard(&board, &high);
                    status = CONTINUE;
                    break; //undo
                case 'r':
                case 'R':
                    status = NEWGAME;
                    break; //restart
                }
                break;
            case WIN:
                printf("   Congratulations\n\n"
                       "[R]estart or Quit\n");
                switch (getch())
                {
                case 'r':
                case 'R':
                    status = NEWGAME;
                    break; //restart
                }
                break;
            }
        } while (status == CONTINUE);
    } while (status == NEWGAME);
    return 0;
}