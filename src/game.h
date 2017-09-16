#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <stdio.h>

void put_ship(char* msg, int pole[][10]);
void put_ansver(char* msg, int pole[][10]);
int chek_shoot(char* msg, int pole[][10]);
int check_ships(int pole[][10]);

int translatex(char* msg, int Nn);
int translatey(char* msg, int Nn);
void input_dest(char* msg);
#endif