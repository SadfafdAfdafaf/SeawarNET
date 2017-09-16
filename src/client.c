#include "client.h"

int init_soket(IPaddress *ip, TCPsocket *tcpsock, SDLNet_SocketSet *set){
	
	if(SDLNet_Init()==-1) {
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		exit(2);
	}
	
	if(SDLNet_ResolveHost(ip, "127.0.0.2", 27015)==-1) {
	   printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
	   exit(1);
	}

	*tcpsock=SDLNet_TCP_Open(ip);
	if(!*tcpsock) {
	   printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
	   exit(2);
	}
	
	*set = SDLNet_AllocSocketSet(1);
	if (!*set) {
		printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
		exit(1); //most of the time this is a major error, but do what you want.
	}	
	
	numused = SDLNet_TCP_AddSocket(*set, *tcpsock);
	if (numused == -1) {
		printf("SDLNet_AddSocket: %s\n", SDLNet_GetError());
		// perhaps you need to restart the set and make it bigger...
	}
	return 1;
}

int game_loop(char* msg, int mypole[][10], int pole[][10]){
	switch (choose){
		case 1:
			numready = SDLNet_CheckSockets(set, 10);
			if (numready){
				if (SDLNet_SocketReady(tcpsock)) {
					result = SDLNet_TCP_Recv(tcpsock, msg, 3);
					if(result <= 0) {
						printf("SDLNet_TCP_Recv: %d\n", result);
						SDLNet_TCP_Close(tcpsock);
						return -1;
					}
					else {							
						printf("OH SHIT. ENEMY ATTAC: %c %c\n", msg[1], msg[2]);
						int k = chek_shoot(msg, mypole);
						msg[0] = '0';
						choose = 0;
						if(k == 1){
							printf("ENEMY DESTROY OUR SHIP IN [%c] [%c]\n", msg[1], msg[2]);
							msg[0] = '1';
							choose = 1;
						}
						int l = check_ships(mypole);
						if(l == -1){
							printf("YOU LOSE\n");
							msg[0] = '2';
						}
						if (choose == 0)
							printf("MISS IN [%c] [%c]\n", msg[1], msg[2]);						
						result = SDLNet_TCP_Send(tcpsock, msg, 3);
						if( result < 3 ) {
							printf( "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
							SDLNet_TCP_Close(tcpsock);
							return -1;
							
						}
						if(l == -1){
							
							return -2;
						}
					}						
				}
			}
			break;
		case 0:
			msg[0] = '0';
			input_dest(msg);
			result = SDLNet_TCP_Send(tcpsock, msg, 3);
			if( result < 3 ) {
				printf( "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
				SDLNet_TCP_Close(tcpsock);
				return -1;
				
			}
			choose = 2;	
			break;
		case 2:
			numready = SDLNet_CheckSockets(set, 10);
			if (numready){				
				if (SDLNet_SocketReady(tcpsock)) {
					result = SDLNet_TCP_Recv(tcpsock, msg, 3);
					if(result <= 0) {
						printf("SDLNet_TCP_Recv: %d\n", result);
						SDLNet_TCP_Close(tcpsock);
						return -1;
					}
					else {
						if(msg[0] == '1'){
							printf("YOU KILL ENEMY SHIP IN [%c] [%c]\n", msg[1], msg[2]);
							put_ansver(msg, pole);
							choose = 0;
						}
						if(msg[0] == '2'){
							printf("SUCCES WINT\n");
							printf("GOOD BYE\n");
							return -2;
						}
						if(msg[0] == '0'){
							printf("MISS IN [%c] [%c]\n", msg[1], msg[2]);
							put_ansver(msg, pole);
							choose = 1;
						}
					}						
				}
			}
			break;
	}
	return 1;
}

void exit_socket(TCPsocket *tcpsock){
	SDLNet_TCP_Close(*tcpsock);
	SDLNet_Quit();
}
