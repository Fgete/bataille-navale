#ifndef DEFINE_H_INCLUDED
#define DEFINE_H_INCLUDED

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

#endif // DEFINE_H_INCLUDED
