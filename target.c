#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#define G 9.81
#define pToN 4.44822 //pounds to newtons constant
#define PI 3.141592654

void target(char array[32][29]);
void printTarget(char array[32][29]);
int menu(int* cash, int arrows[3], double* arrMass, int bow[4], int scores[], double lv, double* strg);
int pay(int* bank, int pay);
void gameSetup(double dist, double vel, double angle);
void calcHit(double azi, double wSpeed, double anglV, double anglH, double n, double dis, double arrM, double height, double* sofHit, double* hofHit);
void figureHitSpot(double sofHit, double hofHit, double tSize, char target[32][29], int* hitS, int* hitF);
void calcScore(int hitS, int hitF, double* str, int* cash, double* lv, int score[9]);

int main(int argc, char* argv[]) {
	int arrs[3] = {10, 0, 0}, points[9] = {0,0,0,0,0,0,0,0,0}, bow[4] = {2, 0, 0, 0};
	int scrt = 0, varCheat = 0, money = 10, correctIn = 0, play, randomVar, hitX, hitY, enoughArrs, enoughStrgth, i;
	double targetSize = 5, hOfShot =  1.8, arrowMass = 0.02, arrowVarX = 50, arrowVarY = 50, strgth = 20.0, level = 1, cheat3;//edited by program, only accessed by cheats
	double distance, windSpeed, azimuth, xOfHit, yOfHit, j;
	double force, angleH, angleV, newtons; //changed by user
	char dim[32][29];
	const char SECRET[8] = "secret";
	char cheat1[10], cheat2[10], cheat4[10], menuCheck, playOrNot;

	srand((int)time(0));

	//printf("\n%s, %d", argv[1], argc);//sanity check

	if(argc > 1){

		//this is for cheating
		if(strcmp(argv[1], "secret") == 0){
			while(strcmp(cheat1, "exit") != 0){
				printf("\n\nEnter cheats(type \"help\" for commands)\n> ");
				scanf(" %s", cheat1);

				if(strcmp(cheat1, "help") == 0){
					printf("\nType change to change values then the value you want to change and to what(EX: change level 10)\nHere are the values you can change(type exit to leave the cheat console):\n");
					printf("\nmoney\t\ttargetSize\theightOfShot\tarrowMass\n\narrowVarX\tarrowVarY\tstrgth\t\tlevel\n\n");
				}
				if(strcmp(cheat1, "change") == 0){
					scanf(" %s %lf", cheat2, &cheat3);
					if(strcmp(cheat2, "money") == 0){
						money = cheat3;
						printf("\nmoney changed to %.0lf", cheat3);
					}else if(strcmp(cheat2, "targetSize") == 0){
						targetSize = cheat3;
						printf("\ntargetSize changed to %.2lf", cheat3);
					}else if(strcmp(cheat2, "heightOfShot") == 0){
						hOfShot = cheat3;
						printf("\nheightOfShot changed to %.2lf", cheat3);
					}else if(strcmp(cheat2, "arrowMass") == 0){
						arrowMass = cheat3;
						printf("\narrowMass changed to %.2lf", cheat3);
					}else if(strcmp(cheat2, "arrowVarX") == 0){
						arrowVarX = cheat3;
						varCheat = 1;
						printf("\narrowVarX changed to %.2lf", cheat3);
					}else if(strcmp(cheat2, "arrowVarY") == 0){
						arrowVarY = cheat3;
						varCheat = 1;
						printf("\narrowVarYchanged to %.2lf", cheat3);
					}else if(strcmp(cheat2, "strgth") == 0){
						strgth = cheat3;
						printf("\nstreight changed to %.2lf", cheat3);
					}else if(strcmp(cheat2, "level") == 0){
						level = cheat3;
						printf("\nlevel changed to %.2lf", cheat3);
					}else{
						printf("\nNot a valid input.");
					}
				}else if(strcmp(cheat1, "unlock") == 0){
					scanf(" %s %s", cheat2, cheat4);
					if(strcmp(cheat2, "all") == 0 && strcmp(cheat4, "bows") == 0){
						printf("\nworking");
						for(i = 1; i < 4; i++){
							bow[i] = 1;
							printf(".");
							sleep(1);
							fflush(stdout);
						}
						sleep(1);
						printf("\n;)");
					}else if(strcmp(cheat2, "max") == 0 && strcmp(cheat4, "arrows") == 0){
						printf("\nworking");
						for(i = 0; i < 3; i++){
							arrs[i] = 999;
							printf(".");
							sleep(1);
							fflush(stdout);
						}
						sleep(1);
						printf("\n;)");
					}
				}
			}
		}
			scrt = 1;
	}


	//print picture
	printf("\n\n\n\n\n\n\n\n^ O|\\ \n|/|+|                         __ \n^ ||/                         || \n  ^                           L| \n-------------------------------- \n\nWelcome to archery, do you want to play?(y/n): ");
	scanf(" %c", &playOrNot);

	if(playOrNot == 'n'){
		return 0;
	}

	while(1 == 1){

		target(dim);	//fills out the target
		if(varCheat == 0){
			arrowVarX = 25*(int)level + 25;	//sets arrow variance in relationship with level
			arrowVarY = 25*(int)level + 25;
		}

		printf("\n\nDo you want to access the menu?(y/n)\n> ");
		scanf(" %c", &menuCheck);
		if(menuCheck == 'y'){
			play  = menu(&money, arrs, &arrowMass, bow, points, level, &strgth);
			if(play == 0){
				printf("\n\nThanks for playing!");
				return 0;
			}
		}

		//Tests to see if you have enough strength to use a bow
		for(i = 1, enoughStrgth = 1; i < 4; i++){
			if(bow[i] == 2){
				if(i == 1){
					if(strgth >= 40){
						enoughStrgth = 1;
					}else{
						enoughStrgth = 0;
					}
				}else if(i == 2){
					if(strgth >= 60){
						enoughStrgth = 1;
					}else{
						enoughStrgth = 0;
					}
				}else if(i == 3){
					if(strgth >= 80){
						enoughStrgth = 1;
					}else{
						enoughStrgth = 0;
					}
				}
			}
		}

		//tests to see if you have enough arrows
		for(i = 0, j = 0.02; i < 3; i++){
			if(arrowMass == j){
				if(arrs[i] > 0){
					enoughArrs = 1;
					arrs[i] -= 1;
				}else{
					enoughArrs = 0;
				}
				break;
			}
			j += 0.01;
		}

		if(enoughArrs == 1 && enoughStrgth == 1){

			//if you went to cheat mode you get to set the distance, wind speed, and azimuth
			if(scrt == 1){
				printf("\nEnter distanace, wind speed, and azimuth\n> ");
				scanf(" %lf %lf %lf", &distance, &windSpeed, &azimuth);
			}else{
				distance = (rand()%20) + 11;
				windSpeed = (rand()%11);
				azimuth = (rand()%360);
			}

			gameSetup(distance, windSpeed, azimuth);

			//takes user input for power and aim angle
			do{
				printf("\n\nEnter draw power(0.0 to 10.0): ");
				scanf(" %lf", &force);
				printf("\nEnter degree vertical you aim at(-180 to 180)(zero is aiming straight): ");
				scanf(" %lf", &angleV);
				printf("\nEnter degree horizontal you aim at(-180 to 180)((-) is to the left (+) is to the right): ");
				scanf(" %lf", &angleH);

				//if you input an invalid input it forces you to input them again
				if(force > 10 || angleV > 180 || angleV < -180 || angleH > 180 || angleH < -180){
					printf("\nThe input was invlaid. Please input it again.");
					correctIn = 0;
				}else{
					correctIn = 1;
				}
			}while(correctIn == 0);


			//calculates the forces of the arrow being pressed
			if(bow[0] == 2){
				newtons = 20.0 * pToN * (force/10);
			}else if(bow[1] == 2){
				newtons = 40.0 * pToN * (force/10);
			}else if(bow[2] == 2){
				newtons = 60.0 * pToN * (force/10);
			}else if(bow[3] == 2){
				newtons = 80.0 * pToN * (force/10);
			}

			calcHit(azimuth, windSpeed, angleV, angleH ,newtons, distance, arrowMass, hOfShot, &xOfHit, &yOfHit);

			//adds random variance to each shot
			if(varCheat == 0){
				randomVar = (rand()%201) - 100;
				arrowVarX = randomVar / arrowVarX;
				randomVar = (rand()%201) - 100;
				arrowVarY = randomVar / arrowVarY;
			}

			xOfHit += arrowVarX;
			yOfHit += arrowVarY;

			//printf("\n%.2lf, %.2lf, %.2lf, %.2lf", yOfHit, xOfHit, arrowVarY, arrowVarX);	//debug

			figureHitSpot(xOfHit, yOfHit, targetSize, dim, &hitX, &hitY);
			printTarget(dim);
			calcScore(hitX, hitY, &strgth, &money, &level, points);

		}else{
			//if there is and issue with your equipment it will tell you what
			printf("\n\nThere are issues with your equipment.");
			if(enoughArrs == 0){
				printf(" You don't have enough arrows.");
			}
			if(enoughStrgth == 0){
				printf(" You don't have enough strength for your bow.");
			}
		}

	}
}

//figures how many points it should assign
void calcScore(int hitS, int hitF, double* str, int* cash, double* lv, int score[9]){
	double disFromCentS, disFromCentF, displacement, lvUp;
	int i, pointVal = 0;

	//lvUp holds the initial level
	lvUp = *lv;

	//finds displacement from the center of the target
	disFromCentS = abs(14 - hitS);
	disFromCentF = abs(14 - hitF);

	displacement = sqrt(pow(disFromCentS,2)+pow(disFromCentF,2));


	if(displacement < 13.59){
		pointVal = 4;
	}
	for(i = 1; i < 7; i++){
		if(displacement < 13.59 - 2 * i){
			pointVal++;
		}else{	//else statement does all the point related calculations
			score[abs(10 - pointVal)]++;
			*cash += pointVal * 5;	//gives 5 dollars for evey point you earn
			*lv += (pointVal * 0.05 / (int)*lv);	//levels done so that you need to get more points to level up as the level increases
			printf("\nCongrats you hit the %d point ring! You get $%d.", pointVal, pointVal * 5);
			break;
		}
	}


	if((int)*lv > (int)lvUp){
		printf(" You also leveled up to %.0lf!", *lv);
		*str += 5;	//strenght increase
	}

	//calculates points by using the displacements
	if(displacement > 13.59){
		if(displacement > 14.59){
			printf("\nYou completely missed the target!");
			score[8]++;
		}else{
			printf("\nAtleast you hit the target!");
			score[7]++;
		}
	}

	//printf("\n%.2lf, %.2lf, %.2lf, %d, %d", displacement, disFromCentS, disFromCentF, pointVal, *cash);	//debug
}

void figureHitSpot(double sofHit, double hofHit, double tSize, char target[32][29], int* hitS, int* hitF){
	double scaleF, scaleS;

	scaleF = (hofHit / tSize) * 29;
	scaleS = (sofHit / tSize) * 15;

	*hitF = round(28 - scaleF);
	*hitS = round(14 + scaleS);

	//printf("\n%d, %d, %.2lf, %.2lf", *hitF, *hitS, scaleF, scaleS);

	if(*hitF >= 0 && *hitF < 29 && *hitS >= 0 && *hitS < 29){
		target[*hitF][*hitS] = '^';
		target[*hitF + 1][*hitS] = '|';
		target[*hitF + 2][*hitS] = '^';
	}
}

void calcHit(double azi, double wSpeed, double anglV, double anglH, double n, double dis, double arrM, double height, double* sofHit, double* hofHit){
	double windVelS, windVelF, radsAz, radsShotV, radsShotH, totFVelocity, totSVelocity, shotVelF, shotVelH, shotVelU, arrVel, timeToHit;
	int hitF, hitS;

	radsAz = azi * (PI/180); //changes degrees to rads
	windVelS = wSpeed * sin(radsAz); //finds wind velocity vector in the side to side directions
	windVelF = wSpeed * cos(radsAz);	//finds wind velocity vector in the front and back directions

	radsShotV = anglV * (PI/180);	//changes angle of shot to rads
	radsShotH = anglH * (PI/180);

	//calculates the intial veloity of the arrow as it is realsed from the bow which
	//we will assume takes 0.01 seconds to apply the full force. The equation is
	//velocity = (force * change in time)/mass
	arrVel = (n * 0.01) / arrM;

	shotVelF = arrVel * cos(radsShotV);	//calculates the horizontal velocity vector of the shot
	totFVelocity = windVelF + shotVelF; //finds total velocity vector
	timeToHit = dis/totFVelocity;	//finds how long it takes to hit the target

	shotVelU = arrVel * sin(radsShotV);	//calcutates the velocity of the arrow in the verical direction
	shotVelH = arrVel * sin(radsShotH);	//calcutates the velocity of the arrow in the horizontal direction

	totSVelocity = shotVelH + windVelS;	//calculates the total velocity vector in the side to side direction

	//calculates the distance from the middle the arrow varries in the horizontal direction
	//I used x(t) = vt
	*sofHit = totSVelocity * timeToHit;

	//calculates how high the shot hits. I used the second antiderivative to find this, the c is the height of the shot taken.
	//the velocity was calculated before. The equation below is under the form of Y(t) = -4.8t^2 + velocity*t + height
	*hofHit = ((-G/2) * pow(timeToHit, 2)) + shotVelU * timeToHit + height;

	//printf("\n%.2lf, %.2lf, %.2lf, %.2lf", *hofHit, *sofHit, totFVelocity, totSVelocity);	//sanity check


}

void gameSetup(double dist, double vel, double angle){
	char direction[10];
	int fixD;

	if(angle >= 360){
		fixD = (int)angle/360;
		angle -= 360*fixD;
	}else if(angle < 0){
		fixD =abs((int)angle/360) + 1;
		angle = 360*fixD - (-1)*angle;
	}

	//afixes N, E, S, or W to the front
	if(angle < 45 || angle > 315){
		direction[0] = 'N';
		direction[1] = '\0';
	}else if(angle > 45 && angle < 135){
		direction[0] = 'E';
		direction[1] = '\0';
	}else if(angle > 135 && angle < 225){
		direction[0] = 'S';
		direction[1] = '\0';
	}else if(angle > 225 && angle < 315){
		direction[0] = 'W';
		direction[1] = '\0';
	}

	//afixes the rest according to the angle
	if(angle > 0 && angle < 90){
		strcat(direction, "NE");
	}else if(angle > 90 && angle < 180){
		strcat(direction, "SE");
	}else if(angle > 180 && angle < 270){
		strcat(direction, "SW");
	}else if(angle > 270 && angle < 360){
		strcat(direction, "NW");
	}

	//prints out the picture of the archer
	printf("\n\n\n\n\n\n\n\n^ O|\\ \n|/|+|                         __ \n^ ||/                         || \n  ^                           L| \n-------------------------------- \n\n");
	printf("The target is %.2lf meters away. The wind is blowing %.2lf degrees %s at %.2lfm/s", dist, angle, direction, vel);
}

int menu(int* cash, int arrows[3], double* arrMass, int bow[4], int scores[9], double lv, double* strg){
	int select = -999, cont = 1, amount = 0, validility = 0, i, j, timeHold, curlAm = 0, totScore = 0, shots;
	time_t t;
	char bufferKill[1000], check[50] = "play", check2[50] = "play", checkStand[50] = "play";

	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

	while(cont == 1){
		printf("\n\n          ------- \n         |Archery| \n          --Menu- \n\n         1) Store \n         2) Lift\n         3) Invetory \n         4) Stats \n         5) Return to Game\n         6) Quit");
		printf("\n_________________________\n\nPlease enter a number: ");
		scanf(" %d", &select);
		;
		//printf("\n\n-- %d --", select);//sanity check

		//this runs the store
		if(select == 1){
			while(1){
				select = -999; //sets select to -999 to test when we need to grab data that is stuck in the buffer
				printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n1) Buy Arrows \n2) Buy Bows\n3) Leave Shop\nYou currently have $%d\n>", *cash);
				scanf(" %d", &select);

				if(select == 1){
					select = -999;
					printf("\n\n\n\n\n\n1)$5 per 20g arrow\n2)$8 per 30g arrow\n3)$10 per 40g arrow\nPick the arrow you want: ");
					scanf(" %d", &select);
					printf("\nHow many arrows would you like?\n> ");
					scanf(" %d", &amount);
					if(select == 1){
						if(pay(cash, 5*amount)){
							arrows[0] += amount;
						}
					}else if(select == 2){
						if(pay(cash, 8*amount)){
							arrows[1] += amount;
						}
					}else if(select == 3){
						if(pay(cash, 10*amount)){
							arrows[2] += amount;
						}
					}else{
						printf("\nNot a valid input.");
						if(select == -999){
							scanf("%s", bufferKill); //kills trash in buffer
						}
					}
					fflush(stdout);
					sleep(2);
				}else if(select == 2){
					while(1){
						select = -999;
						printf("\n\n\n\n\n1) Buy a long bow(40lb) for $100\n2) Buy a recurve bow(60lb) for $150\n3) Buy a compound bow(80lb) for $200\n4) Leave bow shop\n> ");
						scanf("%d", &select);

						if(select == 1){
							if(bow[1] == 1){
								printf("\nYou already have this bow.");
							}else if(pay(cash, 100)){
								bow[1] = 1;
							}
						}else if(select == 2){
							if(bow[2] == 1){
								printf("\nYou already have this bow.");
							}else if(pay(cash, 150)){
								bow[2] = 1;
							}
						}else if(select == 3){
							if(bow[3] == 1){
								printf("\nYou already have this bow.");
							}else if(pay(cash, 200)){
								bow[3] = 1;
							}
						}else if(select == 4){
							break;
						}else{
							printf("\nNot a valid input.");
							if(select == -999){
								scanf("%s", bufferKill);	//kills trash in buffer
							}
						}
					}
				}else if(select == 3){
					break;
				}else{
					printf("\nNot a valid input.");
					if(select == -999){
						scanf("%s", bufferKill);	//kills trash in buffer
					}
				}
			}
		//runs a lifting mini game so that you can increase strength
		}else if(select == 2){

			printf("\nType curl to curl a weight and increase strenght. Type stop to stop curling.");
			time(&t);
			timeHold = t;
			*check = *checkStand;
			curlAm = 0;

			while(strcmp(check, "stop") != 0){
				printf("\n> ");
				scanf(" %s", check);
				if(strcmp(check, "curl") == 0){
					curlAm++;
				}
				time(&t);
			}

			t -= timeHold;
			*strg += (curlAm*5.0)/t +(double)curlAm*0.01*lv;
			printf("\nYour strength has increased to %.2lf.", *strg);

		//runs the invetory so you can change your items
		}else if(select == 3){

			//resets check functions so they don't carry over
			*check = *checkStand;
			*check2 = *checkStand;

			while(strcmp(check, "exit") != 0){
				printf("\n\n\n\nAmount of money: $%d", *cash);
				printf("\n\n---Bows---");

				if(bow[0] >= 1){
					printf("\n");
					if(bow[0] == 2){
						printf("*");
					}
					printf("Training Bow");
				}
				if(bow[1] >= 1){
					printf("\n");
					if(bow[1] == 2){
						printf("*");
					}
					printf("Long Bow");
				}
				if(bow[2] >= 1){
					printf("\n");
					if(bow[2] == 2){
						printf("*");
					}
					printf("Recurve Bow");
				}
				if(bow[3] >= 1){
					printf("\n");
					if(bow[3] == 2){
						printf("*");
					}
					printf("Compound Bow");
				}

				printf("\n\n---Arrows---\n");
				if(*arrMass == 0.02){
					printf("*");
				}
				printf("Amount of 20g arrows: %d\n", arrows[0]);
				if(*arrMass == 0.03){
					printf("*");
				}
				printf("Amount of 30g arrows: %d\n", arrows[1]);
				if(*arrMass == 0.04){
					printf("*");
				}
				printf("Amount of 40g arrows: %d\n", arrows[2]);
				printf("\nChange equipment. Do this by first typing \"select\" and then the name of the item. For instance \"select 20g arrows\". Exit by typing \"exit\". \n> ");

				scanf(" %s", check);
				if(strcmp(check, "select") == 0){
					gets(check2);
				}

				for(i = 0; i < 50; i++){
					check2[i]= toupper(check2[i]);
				}

				//printf("\n%s, %s, %d", check, check2, strcmp(check2, "  LONG BOW"));//sanity check

				//checks to see what the user inputs
				if(strcmp(check2, " TRAINING BOW") == 0){
					for(i = 0; i < 4; i++){
						if(bow[i] == 2){
							bow[i] = 1;
						}
					}
					bow[0] = 2;
				}else if(strcmp(check2, " LONG BOW") == 0){
					for(i = 0; i < 4; i++){
						if(bow[i] == 2){
							bow[i] = 1;
						}
					}
					bow[1] = 2;
				}else if(strcmp(check2, " RECURVE BOW") == 0){
					for(i = 0; i < 4; i++){
						if(bow[i] == 2){
							bow[i] = 1;
						}
					}
					bow[2] = 2;
				}else if(strcmp(check2, " COMPOUND BOW") == 0){
					for(i = 0; i < 4; i++){
						if(bow[i] == 2){
							bow[i] = 1;
						}
					}
					bow[3] = 2;
				}else if(strcmp(check2, " 20G ARROWS") == 0){
					*arrMass = 0.02;
				}else if(strcmp(check2, " 30G ARROWS") == 0){
					printf("\nhello");
					*arrMass = 0.03;
				}else if(strcmp(check2, " 40G ARROWS") == 0){
					*arrMass = 0.04;
				}
			}

		//runs the stat screen
		}else if(select == 4){
			*check = *checkStand;

			while(strcmp(check, "exit") != 0){
				totScore = 0;

				//finds total shot thoug a loop
				for(i = 0, shots = 0; i < 8; i++){
					if(scores[i] > 0){
						shots += scores[i];
					}
				}
				printf("\n\n\nLevel: %.0lf\n\nLevel Progress: %.0lf\n\nStrength: %.2lf\n\nShot taken: %d", lv, (lv/(int)lv - 1)*100, *strg, shots);

				//loops though and adds all the scores up
				for(i = 10, j = 0; i >= 4; i--, j++){
					totScore += scores[j] * i;
				}
				printf("\n\nTotal score: %d\n", totScore);

				//calculates amount of hits per score using a loop
				for(i = 8, j = 0; i >= 2; i--, j++){
					printf("\nAmount of %ds: %d", i+2, scores[j]);
				}

				printf("\nAmount of times the target was hit: %d", shots);
				printf("\nAmount of times the target was missed completely: %d\n\nType exit to leave the stats screen.\n> ", scores[8]);
				scanf(" %s", check);
			}
		}else if(select == 5){
			return  1;
		}else if(select == 6){
			return 0;
		}else{
			printf("\nYou didn't select a valid option");
			if(select == -999){
				scanf("%s", bufferKill);	//kills trash in buffer
			}
		}
	}
}

//finds payment and take money out
int pay(int* bank, int pay){
	if(*bank < pay){
		printf("\nYou don't have enough money! You only have %d! You need %d.", *bank, pay);
		return 0;
	}else{
		*bank -= pay;
		printf("\nYou now have $%d left", *bank);
		return 1;
	}
}

//prints the target with the arrow
void printTarget(char array[32][29]){
	int i, j;

	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

	//loops though arrway and print out each char
	for(i = 0; i < 32; i++){
		for(j = 0; j < 29; j++){
			printf(" %c", array[i][j]);
		}
		printf("\n");
	}
}

void target(char array[32][29]){
	int i, j, k = 0;
	char masterArray[15][15] = {"               ",
														 	 "          ****",
													 	    "      ***    ",
																 "    *   ****",
																	" **  **    ",
									 	   						 "  **  ****",
										   							"*  **    ",
											   						 " *   ***",
									   									"  **   ",
																			 "*  ***",
								   											" *   ",
																				 "  **",
																					"*  ",
																					 "**",
							 													  	" "};

	//loops though and inputs the masterArray data into the data array in th correct positions
	//specifically it fills out the top half of the 2nd quadrant
	for(i = 0; i < 15; i++){
		for(j = i, k = 0; j < 15; j++, k++){
			array[i][j] = masterArray[i][k];
		}
	}

	//fills out the rest of the 2nd quadrant of the target,
	//I assign the (i,j) values of the first part of the array to (j,i),
	//this works because the 90 to 135 is the inverse of 135 to 180.
	for(i = 0; i < 15; i++){
		for(j = i; j < 15; j++){//goes though the the diagonal of the inverse function
			if(i != j){
				array[j][i] = array[i][j];
			}
		}
	}

	//fills out the 1st quadrant of the target with the 2nd quadrant
	//we simple need to reflect this across the y-axis
	for(i = 0; i < 15; i++){
		for(j = 0; j < 14; j++){//we don't need to fill the middle row so we set it to only go to 14
			array[i][28-j] = array[i][j];//subtracting 28 by j flips it
		}
	}

	//fills out the 3rd and 4th quadrants of the target with the 1st and 2nd quandrant
	//we just need to flip it across the x-axis
	for(i = 0; i < 14; i++){//we don't need to fill the middle row so we set it to only go to 14
		for(j = 0; j < 29; j++){
			array[28-i][j] = array[i][j];//subracting 28 by i to flip it
		}
	}

	//makes space below in case the arrow lands below the target
	for(i = 29; i < 32; i++){
		for(j = 0; j < 29; j++){
			array[i][j] = ' ';
		}
	}
}
