#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_net.h>
#include "game.h"

IPaddress ip;
TCPsocket tcpsock;
TCPsocket new_tcpsock;
SDLNet_SocketSet set;
int numused;
int numready;
int phase;
int choose;
int result;

int init_soket(IPaddress *ip, TCPsocket *tcpsock, SDLNet_SocketSet *set);
int game_loop(char* msg, int mypole[][10], int pole[][10]);
void exit_socket(TCPsocket *tcpsock);
void net_quit();

#endif