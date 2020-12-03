typedef struct Case{
    int row;
    int col;
}Case;

typedef struct Bateau{
    int taille;
    int vies;
    struct Case places[5];
}Bateau;

typedef struct Flotte{
    struct Bateau bateaux[5];
}Flotte;

typedef struct Player{
    int id;
    char name[10];
    char mapShip[10][10];
    // char mapHit[10][10];
    struct Case hitList[100];
    int ships[5];
    struct Flotte playerFlotte;
}player;
