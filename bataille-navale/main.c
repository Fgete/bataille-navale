#include <stdio.h>
#include <stdlib.h>

#include "BN_functions.c"

int main()
{
    int gameState = 2;

    player p1, p2;
    p1.id = 1;
    p2.id = 2;

    while (gameState != -1){
        switch (gameState){
        case 0 :
            // INIT
            BN_InitPlayer(&p1);
            BN_InitPlayer(&p2);
            gameState = 1;
            break;
        case 1 :
            // ROUND
            BN_Round(&p1, &p2);
            BN_Round(&p2, &p1);

            BN_Win(&p1, &p2, &gameState);
            break;
        case 2 :
            // MENU
            BN_Menu(&gameState);
            break;
        default :
            printf("\n--- FATAL ERROR ---\n");
            return 1;
        }
    }

    system("cls");
    printf("Au revoir.\n");

    return 0;
}
