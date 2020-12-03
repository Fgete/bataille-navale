#include <stdio.h>
#include <stdlib.h>

#include "BN_struct.c"

#define NSHIP 1 // [1;5]

// PROTOTYPES
// - init
void BN_InitPlayer(struct Player*);
void BN_InitMap(char[10][10]);
void BN_InitListHit(struct Case[100]);
void BN_InitShips(int[5]);
void BN_InitFlotte(struct Flotte*, int[5]);
// - ships
void BN_SetShips(char[10][10], struct Flotte*);
void BN_PlaceShip(char[10][10], struct Bateau*, char);
char BN_GetDirection();
void BN_CreateBateau(struct Bateau*, struct Case, char);
// - display
void BN_PrintMap(char[10][10]);
void BN_PrintBateau(struct Bateau);
// - maps
void BN_CopyMap(char[10][10], char[10][10]);
void BN_TranslateMap(char[10][10], struct Case[100], struct Flotte);
// - round
void BN_Round(struct Player*, struct Player*);
void BN_Win(struct Player*, struct Player*, int*);
int BN_CountHit(struct Flotte, struct Case[100]);
void BN_Menu(int*);
char BN_GetAnswer();
// - case
struct Case BN_CreateCase(int, int);
int BN_CompareCase(struct Case, struct Case);
int BN_CaseInBateau(struct Case, struct Bateau);
int BN_CaseInFlotte(struct Case, struct Flotte);
void BN_PushHit(struct Case, struct Case[100]);
void BN_PrintHitList(struct Case[100]);

// Create case
struct Case BN_CreateCase(int row, int col){
    struct Case c = {row, col};
    return c;
}

// Compare cases
int BN_CompareCase(struct Case a, struct Case b){
    if (a.row == b.row && a.col == b.col)
        return 1;
    else
        return 0;
}

// Compare cases
int BN_CaseInBateau(struct Case c, struct Bateau b){
    for (int i = 0; i < b.taille; i++)
        if (BN_CompareCase(c, b.places[i]))
            return 1;
    return 0;
}

// Compare cases
int BN_CaseInFlotte(struct Case c, struct Flotte f){
    for (int i = 0; i < 5; i++)
        if (BN_CaseInBateau(c, f.bateaux[i]))
            return 1;
    return 0;
}

// Create bateau
void BN_CreateBateau(struct Bateau* b, struct Case origin, char direction){
    for (int i = 0; i < b->taille; i++)
        b->places[i] = origin;
    for (int i = 1; i < b->taille; i++)
        if (direction == 'V')
            b->places[i].row += i;
        else
            b->places[i].col += i;
}

// Initialize player
void BN_InitPlayer(struct Player* p){
    system("cls");
    printf("### BATAILLE NAVALE ###\n");
    printf(" -- Player settings --\n\n");

    printf("Entrez votre nom joueur %d\n", p->id);
    fflush(stdin);
    scanf("%s", p->name);
    printf("\nBonjour %s, nous initialisons vos donnees...\n\n", p->name);
    BN_InitMap(p->mapShip);
    printf("mapShip initialisation [OK]\n");
    BN_InitListHit(p->hitList);
    printf("listHit initialisation [OK]\n");
    BN_InitShips(p->ships);
    printf("ships initialisation [OK]\n");
    BN_InitFlotte(&p->playerFlotte, p->ships);
    printf("flotte initialisation [OK]\n\n");

    printf(" -- ENTER --\n");
    fflush(stdin);
    getchar();

    BN_SetShips(p->mapShip, &p->playerFlotte);
}

// Initialize hit list
void BN_InitListHit(struct Case l[100]){
    for (int i = 0; i < 100; i++){
        l[i].row = -1;
        l[i].col = -1;
    }
}

// Initialize flotte
void BN_InitFlotte(struct Flotte* f, int s[5]){
    for (int i = 0; i < 5; i++){
        f->bateaux[i].taille = s[i];
        f->bateaux[i].vies = s[i];
        for (int j = 0; j < 5; j++){
            f->bateaux[i].places[j].row = -1;
            f->bateaux[i].places[j].col = -1;
        }
    }
}

// Initialize maps
void BN_InitMap(char map[10][10]){
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            map[i][j] = '#';
}

// Copy map
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
void BN_SetShips(char map[10][10], struct Flotte* f){
    for (int nShip = 0; nShip < NSHIP; nShip++){
        system("cls");
        printf("### BATAILLE NAVALE ###\n");
        printf(" -- Ships settings  --\n\n");

        BN_PrintMap(map);

        printf("\nPlacez le bateau %d vertical ou horizontal ? [V/H]\n", nShip + 1);
        BN_PlaceShip(map, &f->bateaux[nShip], BN_GetDirection());
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
void BN_PlaceShip(char map[10][10], struct Bateau* ship, char direction){
    system("cls");
    printf("### BATAILLE NAVALE ###\n");
    printf(" --  Placing ship   --\n\n");

    char tempMap[10][10];
    int x, y;

    BN_CopyMap(map, tempMap);

    // Analyse slot ship space
    if (direction == 'V')
        for (int i = 11 - ship->taille; i < 10; i++)
            for (int j = 0; j < 10; j++)
                if (tempMap[i][j] == '#')
                    tempMap[i][j] = 'x';
    if (direction == 'H')
        for (int i = 0; i < 10; i++)
            for (int j = 11 - ship->taille; j < 10; j++)
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
    printf("\nEntrez la coordonnee verticale :\n");
    fflush(stdin);
    scanf("%d", &x);
    x--;
    printf("\nEntrez la coordonnee en horizontale :\n");
    fflush(stdin);
    scanf("%d", &y);
    y--;

    // Place ship if x & y are valid
   if (tempMap[x][y] == '#'){
        BN_CreateBateau(ship, BN_CreateCase(x, y), direction);
        for (int i = 0; i < ship->taille; i++)
            if (direction == 'V')
                map[x + i][y] = 'O';
            else
                map[x][y + i] = 'O';
    }
    else
        BN_PlaceShip(map, ship, direction);
}

// Round
void BN_Round(struct Player* user, struct Player* target){
    system("cls");
    printf("### BATAILLE NAVALE ###\n");
    printf(" --  Player %d       --\n\n", user->id);

    int x, y;

    char virtualMap[10][10];
    BN_InitMap(virtualMap);
    BN_TranslateMap(virtualMap, user->hitList, target->playerFlotte);
    BN_PrintMap(virtualMap);

    printf("\n%s, attaquez votre adversaire !\n", user->name);
    printf("Entrez une coordonnee verticale :\n");
    scanf("%d", &x);
    x--;
    printf("Entrez une coordonnee horizontale :\n");
    scanf("%d", &y);
    y--;

    // push hit in hitList
    BN_PushHit(BN_CreateCase(x, y), user->hitList);

    system("cls");
    printf("### BATAILLE NAVALE ###\n");
    printf(" --  Player %d       --\n\n", user->id);

    BN_TranslateMap(virtualMap, user->hitList, target->playerFlotte);
    BN_PrintMap(virtualMap);

    if (BN_CaseInFlotte(BN_CreateCase(x, y), target->playerFlotte))
        printf("\nTOUCHE !");
    else
        printf("\nDans l'eau...");

    fflush(stdin);
    getchar();
}

// Lifes Counter
int BN_CountHit(struct Flotte f, struct Case hList[100]){
    struct Case voidSlot = {-1, -1};
    int nHit = 0;
    for (int i = 0; i < 100; i++)
        if (!BN_CompareCase(voidSlot, hList[i]))
            nHit += BN_CaseInFlotte(hList[i], f);
    return nHit;
}

// Victory
void BN_Win(struct Player* p1, struct Player* p2, int* g){
    int p1Hit;
    int p2Hit;

    int totalLifes = 0;
    for (int i = 0; i < NSHIP; i++)
        totalLifes += p1->ships[i];

    p1Hit = BN_CountHit(p2->playerFlotte, p1->hitList);
    p2Hit = BN_CountHit(p1->playerFlotte, p2->hitList);

    if (p1Hit == totalLifes && p2Hit == totalLifes){
        // EQUALITY
        system("cls");
        printf("### BATAILLE NAVALE ###\n");
        printf(" --    EQUALITY !   --\n\n");
        printf("\nImpressionnant ! C'est une egalite parfaite !\n");
        *g = 2;
        fflush(stdin);
        getchar();
    }else if (p1Hit == totalLifes){
        // P1 wins
        system("cls");
        printf("### BATAILLE NAVALE ###\n");
        printf(" --    Player 1 !   --\n\n");
        printf("\nBravo ! Vous avez gagne !\n");
        *g = 2;
        fflush(stdin);
        getchar();
    }else if (p2Hit == totalLifes){
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

// Print ship
void BN_PrintBateau(struct Bateau b){
    printf("\nBateau :\n  taille & vies : %d\n", b.taille);
    for (int i = 0; i < 5; i++)
        printf("  Case %d : %d ; %d\n", i, b.places[i].row, b.places[i].col);
}

// Translate the virtual map using hitList and target playerFlotte
void BN_TranslateMap(char vMap[10][10], struct Case hList[100], struct Flotte tFlotte){
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            struct Case currentSlot = {i, j};
            for (int k = 0; k < 100; k++){
                if (BN_CompareCase(currentSlot, hList[k])){
                    // if hList contain currentSlot, translate tile
                    if (BN_CaseInFlotte(currentSlot, tFlotte)){
                        // if currentSlot is in flotte, translate to HIT
                        vMap[i][j] = 'X';
                    }else{
                        // translate to WATER
                        vMap[i][j] = '~';
                    }
                }
            }
        }
    }
}

// Push Case in hitList
void BN_PushHit(struct Case c, struct Case hList[100]){
    struct Case voidSlot = {-1, -1};
    int done = 0;
    int i = 0;
    while (done != 1){
        if (BN_CompareCase(c, hList[i])){
            done = 1;
        }else{
            if (BN_CompareCase(voidSlot, hList[i])){
                hList[i] = c;
                done = 1;
            }
        }
        if (i == 99){
            done = 1;
        }
        i++;
    }
}

void BN_PrintHitList(struct Case hList[100]){
    for (int i = 0; i < 10; i++){
        printf("%d ; %d\n", hList[i].row, hList[i].col);
    }
}
