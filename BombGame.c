#include <stdio.h>
#include <ncurses/ncurses.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

//the size of the bomb
#define WIDTH 39
#define HEIGHT 13

//struct so i can make an array with different types of variables
typedef struct loginInfo_struct{
	char name[100];
	int defuses;
}loginInfo;

//global array for which wires will go where
int wires[4];

void bomReset(char bomPic[HEIGHT][WIDTH]);
void printBomb(char pic[HEIGHT][WIDTH]);
void changeTime(char bom[HEIGHT][WIDTH], int time, int totTime);
void wirePlacement(char b[HEIGHT][WIDTH], int explode[]);
void wirePic(char b[HEIGHT][WIDTH], int i, char item);
char userFind(loginInfo usr[500], int size, int *userNumber);
char testWire(char pic[HEIGHT][WIDTH], int boom[], int key);

int main() {
	FILE* users;
	FILE* rewrite;
	char bomb[HEIGHT][WIDTH] = {"* * * * * * * * * * * * * * * * * * * *",
															"*                                     *",
															"*              0 0 : 0 0              *",
															"*                                     *",
															"*     a                         1     *",
															"*                                     *",
															"*     b                         2     *",
															"*                                     *",
															"*     c                         3     *",
															"*                                     *",
															"*     d                         4     *",
															"*                                     *",
															"* * * * * * * * * * * * * * * * * * * *"};

	char c = 'n', test;
	char find, usrName[50];
	int diff, timeSoFar = 0, totTime = 90, i = 0, j;
	int usrNum, boom[4] = {0,0,0,0};
	long init, final;
	struct timespec start;
	loginInfo login[500];

	//seeds time
	srand((int)time(0));

	//opens file to read
	users = fopen("login.txt", "r");

	//scans the text infor into a array or structs
	while(fscanf(users, "%s %d", login[i].name, &login[i].defuses) == 2){
		i++;
		//printf("\ni=%d %s %d", i, login[i].name, &login[i].defuses ); 	//sanity check
	}

	//finds the user in the list if they exist
	find = userFind(login, i, &usrNum);

	if(find == 'y'){
		printf("Hello %s. You have successfully defused %d bomb(s).", login[usrNum].name, login[usrNum].defuses);
		usrNum = i;
	}else{
		printf("Hello %s. You are a new user.", login[usrNum].name);
	}

	fflush(stdout);
	sleep(1);
	//printf("%s, %d", login[usrNum].name, login[usrNum].defuses);	//sanity check

	initscr();
	refresh();
	//lets me use getch to get input without stopping the program
	nodelay(stdscr, TRUE);
	do{
		//transfers the picture of the bomb and an array about which wire will do what.
		wirePlacement(bomb, boom);

		//gets begining time
		clock_gettime(CLOCK_REALTIME, &start);
		init = start.tv_sec;

		do{

			//gets time diff
			clock_gettime(CLOCK_REALTIME, &start);
			diff = start.tv_sec - init;

			if(diff >= 1){
				timeSoFar += diff;

				if(i > totTime){
					endwin();
					printf("You blew up!");
					return -999;
				}

				changeTime(bomb, timeSoFar, totTime);
				printBomb(bomb);
				init = start.tv_sec;
			}

			refresh();

			//tests to see if you press any buttons
			mvprintw(14, 0, "Which wire do you want to cut?(q to quit)");
			c = getch();

			if(c == 'q'){
				break;
			}else if(c == 'a'){
				test = testWire(bomb, boom, 0);
			}else if(c == 'b'){
				test = testWire(bomb, boom, 1);
			}else if(c == 'c'){
				test = testWire(bomb, boom, 2);
			}else if(c == 'd'){
				test = testWire(bomb, boom, 3);
			}

			if(test == 's'){	//adds one to your defuses

				mvprintw(14, 0, "You defused the bomb! Good Job.          ");
				login[usrNum].defuses++;//adds on the amount of defuses
				break;

			}else if(test == 'b'){

				mvprintw(14, 0, "You cut the wrong wire!                  ");
				break;

			}

			refresh();
		}while(1 == 1);//inner loop to do bomb things

		//resets everything for new bomb
		if( c != 'q'){
			mvprintw(15, 0, "Starting new game...");
			refresh();
			sleep(1);
			mvprintw(15, 0, "                     ");//clears out the line
			refresh();
			test = 'n';
			bomReset(bomb);//resets the middle of the bomb
			for(j = 0; j<4;j++){
				boom[j] = 0;
			}
		}
	}while(c != 'q');//seconds outer loop just to run the game

	endwin();

	printf("\n\nSaving..");
	fflush(stdout);

	rewrite = fopen("login.txt", "w+");
	for(j = 0; j < usrNum; j++){
		fprintf(rewrite, "%s %d\n", login[j].name, login[j].defuses);
		//printf("\nj=%d %s %d", j, login[j].name, login[j].defuses);//sanity check
	}

	//closes the file things
	fclose(users);
	fclose(rewrite);
	sleep(1);

	printf("\nThanks for playing!");


}

void bomReset(char bomPic[HEIGHT][WIDTH]){
	int i, j;

	//loop that fills the inside with blank spaces
	for(i = 4; i <= 10; i++){
		for(j = 8; j <= 30; j += 2){
			bomPic[i][j] = ' ';
		}
	}
}


char testWire(char pic[HEIGHT][WIDTH], int boom[], int key){
	int i;

	mvprintw(15, 0, "                         ");

	if(boom[key] == 0){

		wirePic(pic, key, ' ');

		for(i = 0; i < 4;i++){
			if(key != i && boom[i] != -1){
				wirePic(pic, i, '*');
			}
		}

		//makes the boom array to -1 so that in the future we know it has been cut
		boom[key] = -1;
		mvprintw(15, 0, "Nothing happened");

		return 'n';	//returns that the wire didn't do anything

	}else if(boom[key] == 1){
		return 's';	//returns taht the bomb is defused
	}else if(boom[key] == 2){
		return 'b';	//returns that bomb exploded
	}else if(boom[key] == -1){
		//if it is already gone then it will tell you
		mvprintw(15, 0, "You already cut that");
		return 'n';
	}
}

//used to fill the inside of the target with it's wires
void wirePic(char b[HEIGHT][WIDTH], int i, char item){
	int j;

	//goes though the array and puts the symbol as nesserary
	if(i == wires[i]){
		for(j = 8; j <= 30; j += 2){
			b[4 + i*2][j] = item;
		}
	}else if(i == wires[i]-1){
		for(j = 8; j <= 14; j += 2){
			b[4+i*2][j] = item;
			b[5+i*2][j+8] = item;
			b[6+i*2][j+16] = item;
		}
	}else if(i == wires[i]-2){
		for(j = 16; j <= 22; j += 2){
			b[i*2+6][j] = item;
		}
		for(j = 8; j <= 10; j += 2){
			b[i*2+4][j] = item;
			b[i*2+5][j+4] = item;
			b[i*2+7][j+16] = item;
			b[i*2+8][j+20] = item;
		}
	}else if(i == wires[i]-3){
		b[i*2+4][8] = item;
		b[i*2+10][30] = item;
		for(j = 10; j <= 12; j += 2){
			b[i*2+5][j] = item;
			b[i*2+6][j+4] = item;
			b[i*2+7][j+8] = item;
			b[i*2+8][j+12] = item;
			b[i*2+9][j+16] = item;
		}
	}else if(i == wires[i]+1){
		for(j = 8; j <= 14; j += 2){
			b[4+i*2][j] = item;
			b[3+i*2][j+8] = item;
			b[2+i*2][j+16] = item;
		}
	}else if(i == wires[i]+2){
		for(j = 16; j<=22; j += 2){
			b[i*2+2][j] = item;
		}
		for(j = 8; j <= 10; j += 2){
			b[i*2+4][j] = item;
			b[i*2+3][j+4] = item;
			b[i*2+1][j+16] = item;
			b[i*2+0][j+20] = item;
		}
	}else if(i == wires[i]+3){
		b[i*2+4][8] = item;
		b[i*2-2][30] = item;
		for(j = 10; j <= 12; j += 2){
			b[i*2+3][j] = item;
			b[i*2+2][j+4] = item;
			b[i*2+1][j+8] = item;
			b[i*2+0][j+12] = item;
			b[i*2-1][j+16] = item;
		}
	}
}

void wirePlacement(char b[HEIGHT][WIDTH], int explode[]){
	int i, holdExplode, holdSafe;
	char syb = '*';

	//finds where each wire goes too and puts that into a picture
	for(i = 0; i < 4; i++){
		wires[i] = rand()%4;
		//mvprintw(16,0,"%d", wires[i]);
		//refresh();	//sanity check
		wirePic(b,i,syb);
	}

	holdExplode = rand()%4;
	explode[holdExplode] = 2;	//this assigns a random wire to explode

	//makes sure that the same wire doesn't get picked twice
	do{
		holdSafe = rand()%4;
	}while(holdExplode == holdSafe);

	explode[holdSafe] = 1;	//assigns which wire will defuse it
}

char userFind(loginInfo usr[500], int size, int* userNumber){
	int i;
	char name[50], left;

	printf("Enter username: ");
	scanf(" %s", name);

	//searches the struct for any similar users
	for(i = 0; i < size; i++){
		if(strcmp(name, usr[i].name) == 0){
			*userNumber = i;
			return 'y';
		}
	}
	*userNumber = i+1;
	strcpy(usr[*userNumber-1].name,name);
	usr[*userNumber-1].defuses = 0;
	return 'n';
}

//this function runs the timer
void changeTime(char bom[HEIGHT][WIDTH], int t, int totTime){
	char tSecTens, tSecDigs, tMi;
	int tSecTen, tSecDig, tSec, tMin, tSecHold, tMinHold, tLeft;

	//finds time left in seconds
	tLeft = totTime - t;

	//turns that into seconds and minutes
	tMin = tLeft/60;
	tSec = tLeft - tMin*60;

	//calculates individual digits
	tSecTen = tSec/10;
	tSecDig = tSec - tSecTen*10;

	tSecTens = tSecTen + '0';
	tSecDigs = tSecDig + '0';
	tMi = tMin + '0';

	bom[2][17] = tMin + '0';
	bom[2][21] = tSecTen + '0';
	bom[2][23] = tSecDig + '0';

	//mvprintw(12,0,"%d : %d %d, %d", tMin, tSecTen, tSecDig, t);
	//refresh();
}

//prints the picture out
void printBomb(char pic[HEIGHT][WIDTH]){
	int i, j;

	for(i = 0; i < HEIGHT; i++){
		for(j = 0; j < WIDTH; j++){
			mvaddch(i,j, pic[i][j]);
			refresh();
		}
	}
}
