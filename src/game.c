#include "game.h"

void put_ship(char* msg, int pole[][10]){
	int xcord, ycord;
	
	xcord = (int)msg[1] - (int)'a';
	ycord = (int)msg[2] - (int)'0';
	pole[xcord][ycord] = 1;
}

void put_ansver(char* msg, int pole[][10]){
	int xcord, ycord;
	
	xcord = (int)msg[1] - (int)'a';
	ycord = (int)msg[2] - (int)'0';
	pole[xcord][ycord] = 0;
}

int chek_shoot(char* msg, int pole[][10]){
	int xcord, ycord;
	
	xcord = (int)msg[1] - (int)'a';
	ycord = (int)msg[2] - (int)'0';
	
	if(pole[xcord][ycord] == 1){
		pole[xcord][ycord] = 0;
		return 1;		
	}else
		return 0;	
}

int check_ships(int pole[][10]){
	int k = 0;
	for(int i = 0; i<10; i++)
        for(int j = 0; j<10; j++)
        {	
			if(pole[i][j] == 1)
				k++;
        }
		
	if (k == 0)
		return -1;
	else
		return 0;
}

int translatex(char* msg, int Nn){
	int xcord;
	xcord = (int)msg[1] - (int)'a';
	xcord *= (int)(Nn/10);
	xcord += (int)(Nn/20);
	return xcord;
}

int translatey(char* msg, int Nn){
	int ycord;
	ycord = (int)msg[2] - (int)'0';
	ycord *= (int)(Nn/10);
	ycord += (int)(Nn/20);
	return ycord;
}

void input_dest(char* msg){
	printf("Input your desteny\n");
	fflush(stdin);
	msg[0] = '0';
	scanf("%c%c", &msg[1], &msg[2]);	
}