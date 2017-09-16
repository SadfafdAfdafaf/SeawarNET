#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_net.h>
#include "game.h"

IPaddress ip;
TCPsocket tcpsock;
SDLNet_SocketSet set;
int numused;
int numready;

int choose;
int result;

int init_soket(IPaddress *ip, TCPsocket *tcpsock, SDLNet_SocketSet *set);
int game_loop(char* msg, int mypole[][10], int pole[][10]);
void exit_socket(TCPsocket *tcpsock);


#endif