#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

#define NSHIP 5 // [1;5]

typedef struct player{
    int id;
    char name[10];
    char mapShip[10][10];
    char mapHit[10][10];
    int ships[5];
}player;

// PROTOTYPES
// - init
void BN_InitPlayer(struct player*);
void BN_InitMap(char[10][10]);
void BN_InitShips(int[5]);
// - ships
void BN_SetShips(char[10][10], int[5]);
void BN_PlaceShip(char[10][10], int, char);
char BN_GetDirection();
// - display
void BN_PrintMap(char[10][10]);
// - maps
void BN_CopyMap(char[10][10], char[10][10]);
// - round
void BN_Round(struct player*, struct player*);
void BN_Win(struct player*, struct player*, int*);
void BN_Menu(int*);
char BN_GetAnswer();

// Initialize player
void BN_InitPlayer(struct player* p){
    system("cls");
    printf("### BATAILLE NAVALE ###\n");
    printf(" -- Player settings --\n\n");

    printf("Entrez votre nom joueur %d\n", p->id);
    fflush(stdin);
    scanf("%s", p->name);
    printf("\nBonjour %s, nous initialisons vos donnees...\n\n", p->name);
    BN_InitMap(p->mapShip);
    printf("mapShip initialisation [OK]\n");
    BN_InitMap(p->mapHit);
    printf("mapHit initialisation [OK]\n");
    BN_InitShips(p->ships);
    printf("ships initialisation [OK]\n\n");

    printf(" -- ENTER --\n");
    fflush(stdin);
    getchar();

    BN_SetShips(p->mapShip, p->ships);
}

// Initialize maps
void BN_InitMap(char map[10][10]){
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            map[i][j] = '#';
}

void BN_CopyMap(char mapSource[10][10], char mapTarget[10][10]){
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            mapTarget[i][j] = mapSource[i][j];
}

// Initialize ships
void BN_InitShips(int ships[5]){
    ships[0] = 2;
    ships[1] = 3;
    ships[2] = 3;
    ships[3] = 4;
    ships[4] = 5;
}

// Print maps
void BN_PrintMap(char map[10][10]){
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++)
            printf("%c", map[i][j]);
    printf("\n");
    }
}

// User set ships on the map
void BN_SetShips(char map[10][10], int ships[5]){
    for (int nShip = 0; nShip < NSHIP; nShip++){
        system("cls");
        printf("### BATAILLE NAVALE ###\n");
        printf(" -- Ships settings  --\n\n");

        BN_PrintMap(map);

        printf("\nPlacez le bateau %d vertical ou horizontal ? [V/H]\n", nShip + 1);
        BN_PlaceShip(map, ships[nShip], BN_GetDirection());
    }

    system("cls");
    printf("### BATAILLE NAVALE ###\n");
    printf(" -- Ships settings  --\n\n");

    BN_PrintMap(map);

    printf("\nVos bateaux sont pret !\n");
    fflush(stdin);
    getchar();
}

// Get direction from user
char BN_GetDirection(){
    char answer = 'x';
    while (answer != 'v' && answer != 'V' && answer != 'h' && answer != 'H'){
        fflush(stdin);
        answer = getchar();
    }
    return toupper(answer);
}

// Get answer from user
char BN_GetAnswer(){
    char answer = 'x';
    while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N'){
        fflush(stdin);
        answer = getchar();
    }
    return toupper(answer);
}

// User place ship number x
void BN_PlaceShip(char map[10][10], int ship, char direction){
    system("cls");
    printf("### BATAILLE NAVALE ###\n");
    printf(" --  Placing ship   --\n\n");

    char tempMap[10][10];
    int x, y;

    BN_CopyMap(map, tempMap);

    // Analyse slot ship space
    if (direction == 'V')
        for (int i = 11 - ship; i < 10; i++)
            for (int j = 0; j < 10; j++)
                if (tempMap[i][j] == '#')
                    tempMap[i][j] = 'x';
    if (direction == 'H')
        for (int i = 0; i < 10; i++)
            for (int j = 11 - ship; j < 10; j++)
                if (tempMap[i][j] == '#')
                    tempMap[i][j] = 'x';
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            if (map[i][j] == '#'){
                if (i > 0){
                    if (map[i-1][j] != '#')
                        tempMap[i][j] = 'x';
                    if (j > 0)
                        if (map[i-1][j-1] != '#')
                            tempMap[i][j] = 'x';
                    if (j < 9)
                        if (map[i-1][j+1] != '#')
                            tempMap[i][j] = 'x';
                }
                if (i < 9){
                    if (map[i+1][j] != '#')
                        tempMap[i][j] = 'x';
                    if (j > 0)
                        if (map[i+1][j-1] != '#')
                            tempMap[i][j] = 'x';
                    if (j < 9)
                        if (map[i+1][j+1] != '#')
                            tempMap[i][j] = 'x';
                }
                if (j > 0)
                    if (map[i][j-1] != '#')
                        tempMap[i][j] = 'x';
                if (j < 9)
                    if (map[i][j+1] != '#')
                        tempMap[i][j] = 'x';
            }

    BN_PrintMap(tempMap);

    // Get x & y
    printf("\nEntrez la coordonnee en X :\n");
    fflush(stdin);
    scanf("%d", &x);
    x--;
    printf("\nEntrez la coordonnee en Y :\n");
    fflush(stdin);
    scanf("%d", &y);
    y--;

    // Place ship if x & y are valid
    if (tempMap[x][y] == '#')
        for (int i = 0; i < ship; i++)
            if (direction == 'V')
                map[x + i][y] = 'O';
            else
                map[x][y + i] = 'O';
    else
        BN_PlaceShip(map, ship, direction);
}

void BN_Round(struct player* user, struct player* target){
    system("cls");
    printf("### BATAILLE NAVALE ###\n");
    printf(" --  Player %d       --\n\n", user->id);

    int x, y;

    BN_PrintMap(user->mapHit);

    printf("\n%s, attaquez votre adversaire !\n", user->name);
    printf("Entrez une coordonnee en X :\n");
    scanf("%d", &x);
    x--;
    printf("Entrez une coordonnee en Y :\n");
    scanf("%d", &y);
    y--;

    if (target->mapShip[x][y] != '#'){
        user->mapHit[x][y] = 'X';

        system("cls");
        printf("### BATAILLE NAVALE ###\n");
        printf(" --  Player %d       --\n\n", user->id);

        BN_PrintMap(user->mapHit);

        printf("\nTOUCHE !\n");
    }else{
        user->mapHit[x][y] = '~';

        system("cls");
        printf("### BATAILLE NAVALE ###\n");
        printf(" --  Player %d       --\n\n", user->id);

        BN_PrintMap(user->mapHit);

        printf("\nDans l'eau...\n");
    }
    fflush(stdin);
    getchar();
}

// Victory
void BN_Win(struct player* p1, struct player* p2, int* g){
    int p1Win = 1;
    int p2Win = 1;

    // P1 win ?
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            if (p2->mapShip[i][j] == 'O')
                if (p1->mapHit[i][j] != 'X')
                    p1Win = 0;

    // P2 win ?
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            if (p1->mapShip[i][j] == 'O')
                if (p2->mapHit[i][j] != 'X')
                    p2Win = 0;

    if (p1Win == 1 && p2Win == 1){
        // EQUALITY
        system("cls");
        printf("### BATAILLE NAVALE ###\n");
        printf(" --    EQUALITY !   --\n\n");
        printf("\nImpressionnant ! C'est une egalite parfaite !\n");
        *g = 2;
        fflush(stdin);
        getchar();
    }else if (p1Win == 1){
        // P1 wins
        system("cls");
        printf("### BATAILLE NAVALE ###\n");
        printf(" --    Player 1 !   --\n\n");
        printf("\nBravo ! Vous avez gagne !\n");
        *g = 2;
        fflush(stdin);
        getchar();
    }else if (p2Win == 1){
        // P2 wins
        system("cls");
        printf("### BATAILLE NAVALE ###\n");
        printf(" --    Player 2 !   --\n\n");
        printf("\nBravo ! Vous avez gagne !\n");
        *g = 2;
        fflush(stdin);
        getchar();
    }
}

// Menu
void BN_Menu(int* g){
    system("cls");
    printf("### BATAILLE NAVALE ###\n");
    printf(" --      Menu       --\n\n");
    printf("Voulez-vous jouer ? [Y/N]\n");

    char a;
    a = BN_GetAnswer();

    if (a == 'Y')
        *g = 0;
    else
        *g = -1;
}
