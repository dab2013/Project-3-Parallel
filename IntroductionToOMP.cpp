// OpenMP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <omp.h>
#include <cmath>
#include <ctime>
#include <math.h>


void integral(void);
void sort(void);
void checkers(void);
void fillBoardandPlacePieces(void); 
int findPiece(int vert, int horz);
bool checkmove(struct pieces colorPiece, int vert, int horz);
bool midpointOpposingPlayerPiece(int vert1, int vert2, int horz1, int horz2, char color);
int numPieces(struct pieces colorPiece[12]);
double checkForPieces(struct pieces blackPiece);
double checkLeft(int vert, int horz); 
double checkRight(int vert, int horz);
int chooseAPieceForMoving(double moveType[12]);
void computerBrain(void);
void computerMove(double moveType, struct pieces blackPiece);


static long num_steps = 100000;
double step, storedValues[100];
int placement = 0;
#define NUM_THREADS 2

struct boardspace {
	bool canHavePiece;
	bool hasPiece;
	char colorOnSpace;
	bool edge;
	bool kingspace;
};
struct boardspace place[8][8];

struct pieces {
	char color;
	bool king;
	bool onTheBoard;
	int placeOnBoard_Vert;
	int placeOnBoard_Horz;
};
struct pieces blackPieces[12];
struct pieces redPieces[12];


int main()
{
	int value = 0;

	while (value != 3)
	{
		system("cls");
		std::cout << "1: Find the integral \n";
		std::cout << "2: Sort Values \n";
		std::cout << "3: Checkers \n";
		std::cout << "4: Exit  \n";
		printf("Enter your choice: ");
		std::cin >> value;

		if (value == 1)
		{
			integral();
		}

		else if (value == 2)
		{
			sort();
		}
		else if (value == 3)
		{
			checkers();
		}
	}

    return 0;
}

void integral(void) {
	int i, nthreads, input_x, input_n;
	double x, answer, sum[NUM_THREADS];

	step = 1.0 / (double)num_steps;
	omp_set_num_threads(NUM_THREADS);
	std::cout << "This is only a integral calculator from 0 to 1 \n";
	std::cout << "1: nx \n";
	std::cout << "2: 1/(x+n)\n";
	std::cout << "3: 1/(x-n)\n";
	std::cout << "4: 1/(n-x)\n";
	std::cout << "5: x^n\n";
	std::cout << "Please select the type of x: ";
	std::cin >> input_x;
	std::cout << "Please input the value of n: ";
	std::cin >> input_n;
#pragma omp parallel
	{
		int i, id, nthrds;
		double x;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();

		if (id == 0) nthreads = nthrds;
		for (i = id, sum[id] = 0.0; i < num_steps; i += nthrds)
		{
			x = (i + .5)*step;

			if (input_x == 5 && input_n == 0)
				sum[id] += input_n * 1;
			else if (input_n != 0) {
				if (input_x == 1)
					sum[id] += input_n * x;
				else if (input_x == 2)
					sum[id] += 1 / (input_n + x);
				else if (input_x == 3)
					sum[id] += 1 / (x - input_n);
				else if (input_x == 4)
					sum[id] += 1 / (input_n - x);
				else if (input_x == 5)
					sum[id] += pow(x, input_n);
			}
		}
	}

	for (i = 0, answer = 0.0; i < nthreads; i++)
	{
		answer += step * sum[i];
		//std::cout << "\n\n" << pi  << ", " << sum[i] << ", " << step << "\n";
	}

	std::cout << "\n\nThe integral is equal to: " << answer;

	char store = 'n';
	std::cout << "\nWould you like to store this value? [y/n] ";
	std::cin >> store;
	if (store == 'y' || store == 'Y') {
		storedValues[placement] = answer;
		if (placement == 99)
			placement = 0;
		else
			placement++;
	}
	
	std::cout << "\n\nPress enter to continue.... ";

	std::getchar();	std::getchar();

}

void sort(void) {

	int i, sortInput, loopValue = 0;
	const int n_b = 100;
	double b[n_b] = { };
	std::cout << "1: Input Values \n";
	std::cout << "2: Use Stored Integral Values\n";
	std::cout << "3: Randomize\n";
	std::cout << "\nPlease select which one you would like to do: ";
	std::cin >> sortInput;

	if (sortInput == 1) {
		std::cout << "Please select a number from 1 - 10 for length: ";
		std::cin >> loopValue;
		for (int i = 0; i < loopValue; i++) {
			double temp1;
			std::cout << "Please choose a number: ";
			std::cin >> temp1;
			b[i] = temp1;
		}
	}
	else if (sortInput == 2) {
		memcpy(b, storedValues, sizeof(storedValues));
		loopValue = placement;
	}
	else if (sortInput == 3) {
		std::cout << "Please select a number from 1 - 100 for length: ";
		std::cin >> loopValue;
		double temp[100];
		for (int i = 0; i < loopValue; i++) {
			double temp1 = (rand() % 200);
			b[i] = temp1;
		}
	}

	for (int phase = 0; phase < n_b; phase++) {
		if (phase % 2 == 0) {
#pragma omp parallel for num_threads(8) default(none) shared (b, n_b) private(i)
			for (i = 1; i < n_b; i += 2) {
				if (b[i - 1] > b[i]) {
					double temp = b[i - 1];
					b[i - 1] = b[i];
					b[i] = temp;
				}
			}
		}
		else {
#pragma omp parallel for num_threads(8) default(none) shared (b, n_b) private(i)
			for (i = 1; i < n_b - 1; i += 2) {
				if (b[i] > b[i + 1]) {
					double temp = b[i + 1];
					b[i + 1] = b[i];
					b[i] = temp;
				}
			}
		}
	}


	for (int i = loopValue - 1; i >= 0; i--)
	{
		std::cout << b[i];
		if (i != 0)
			std::cout << ", ";
	}

	std::cout << "\n\nPress enter to continue.... ";

	std::getchar();	std::getchar();
}

void checkers(void) {
	int win = -1;

	while (win != -1) {
		//while (numPieces(redPieces) != 0 || numPieces(blackPieces) != 0){
		system("cls");
		fillBoardandPlacePieces();
		int vert, horz, vertMove, horzMove;

		std::cout << "Please input the value of the piece (vert): ";
		std::cin >> vert;
		std::cout << "Please input the value of the piece (horz): ";
		std::cin >> horz;
		int pieceBeingMoved = findPiece(vert, horz);

		std::cout << "Please input the value of where the piece is being moved (vert): ";
		std::cin >> vertMove;
		std::cout << "Please input the value of where the piece is being moved (horz): ";
		std::cin >> horzMove;

		std::cout << checkmove(redPieces[pieceBeingMoved], vertMove, horzMove);


		std::cout << "End? ";
		std::cin >> win;
		std::getchar();	std::getchar();
	}

	fillBoardandPlacePieces();
	/*std::cout << numPieces(redPieces) << "\n";
	redPieces[1].onTheBoard = false;
	std::cout << numPieces(redPieces) << "\n";*/
	for (int i = 0; i < 12; i++) {
		std::cout << i <<" - " << blackPieces[i].placeOnBoard_Vert << "," << blackPieces[i].placeOnBoard_Horz << "\n";
	}
		computerBrain();

	for (int i = 0; i < 12; i++) {
		std::cout<< i << " - " << blackPieces[i].placeOnBoard_Vert << "," << blackPieces[i].placeOnBoard_Horz << "\n";
	}

	/*std::cout << "can have a piece? : " << place[vert][horz].canHavePiece << "\n";
	std::cout << "has piece? : " << place[vert][horz].hasPiece << "\n";
	std::cout << "color : " << place[vert][horz].colorOnSpace << "\n";
	std::cout << "edge? : " << place[vert][horz].edge << "\n";
	std::cout << "king space? : " << place[vert][horz].kingspace << "\n\n";*/

	/*for (int i = 0; i < 12; i++) {
		std::cout << "king? : " << redPieces[i].king << "\n";
		std::cout << "on the board? : " << redPieces[i].onTheBoard << "\n";
		std::cout << "place (vert) : " << redPieces[i].placeOnBoard_Vert << "\n";
		std::cout << "place (horz) : " << redPieces[i].placeOnBoard_Horz << "\n\n";
	}*/
	std::getchar();	std::getchar();

}

bool checkmove(struct pieces colorPiece, int vert, int horz) {
	if (place[vert][horz].canHavePiece) {
		if (colorPiece.king)
		{

		}
		else {
			if (colorPiece.color == 'r' && vert > colorPiece.placeOnBoard_Vert && horz > colorPiece.placeOnBoard_Horz) { // && space == backwards
				if (place[vert][horz].hasPiece == false)
					return true;
				else if (vert - colorPiece.placeOnBoard_Vert == 2 && abs(horz - colorPiece.placeOnBoard_Horz) == 2 && midpointOpposingPlayerPiece(vert, colorPiece.placeOnBoard_Vert, horz, colorPiece.placeOnBoard_Horz, colorPiece.color)) {
					return true;
				}
			}

		}
	}

	return false;
}

bool midpointOpposingPlayerPiece(int vert1, int vert2, int horz1, int horz2, char color) {
	const int vert = (vert1 + vert2) / 2;
	const int horz = (horz1 + horz2) / 2;

	if (place[vert][horz].colorOnSpace != color) {
		int opposingPiece = findPiece(vert, horz);
		if (color == 'r') {
			blackPieces[opposingPiece].onTheBoard = false;
			place[vert][horz].hasPiece = false;
		}
		else {
			redPieces[opposingPiece].onTheBoard = false;
			place[vert][horz].hasPiece = false;
		}
		return true;
	}
	return false;
}

int findPiece(int vert, int horz) {
	for (int i = 0; i < 12; i++) {
		if (redPieces[i].placeOnBoard_Vert == vert && redPieces[i].placeOnBoard_Horz == horz) {
			return i;
		}
	}

	return -1;
}

void fillBoardandPlacePieces(void) {
	int redCounter = 0;
	int blackCounter = 0;
	for (int i = 0; i < 8; i++) {
		//To allow quicker runtime
		#pragma omp parallel for 
		for (int j = 0; j < 8; j++) {
			if (j % 2 == 1 && i%2 == 0) {
				place[i][j].canHavePiece = false;
			}
			else if (j % 2 == 0 && i % 2 == 1) {
				place[i][j].canHavePiece = false;
			}
			else {
				place[i][j].canHavePiece = true;
				if (i >= 0 && i <= 2)
				{
					//struct boardspace
					place[i][j].hasPiece = true; 
					place[i][j].colorOnSpace = 'r';

					//struct pieces
				#pragma omp critical
					{
						redPieces[redCounter].color = 'r';
						redPieces[redCounter].king = false;
						redPieces[redCounter].onTheBoard = true;
						redPieces[redCounter].placeOnBoard_Vert = i;
						redPieces[redCounter].placeOnBoard_Horz = j;

						redCounter++;
					}

				}
				else if (i >= 5 && i <= 7)
				{					
					//struct boardspace
					place[i][j].hasPiece = true;
					place[i][j].colorOnSpace = 'b';

					//struct pieces
				#pragma omp critical
					{
						blackPieces[blackCounter].color = 'b';
						blackPieces[blackCounter].king = false;
						blackPieces[blackCounter].onTheBoard = true;
						blackPieces[blackCounter].placeOnBoard_Vert = i;
						blackPieces[blackCounter].placeOnBoard_Horz = j;

						blackCounter++;
					}
				}
			}
			if (i == 0 || j== 0 || i ==7 || j==7) {
				place[i][j].edge = true;
			}
			if (i == 0 || i == 7 ) {
				place[i][j].kingspace = true;
			}
		}
	}

}

int numPieces(struct pieces colorPiece[12]) {
	int i, totalNumPieces = 0; 
#pragma omp parallel for
	for (i = 0; i < 12; i ++) {
		if (colorPiece[i].onTheBoard) {
		#pragma omp critical
			totalNumPieces ++;
		}
	}

	return totalNumPieces;
}

void computerBrain(void) {
	double moveType[12];

	//MAKE A SIMPLE COMPUTER BRAIN
	#pragma omp parallel for 
		for (int i = 0; i < 12; i++) {
			//check if piece is on the board
			if (blackPieces[i].onTheBoard) {
				//check to see if piece is blocked by same color piece
				//if not blocked can it attack or move (first piece that can attack move)
				moveType[i] = checkForPieces(blackPieces[i]);
			}
		}
#pragma omp barrier
		{
		//after looking through the moves select a piece
		int pieceNum =  chooseAPieceForMoving(moveType);

		//move piece
			computerMove(moveType[pieceNum], blackPieces[pieceNum]);
		}
		//std::cout<< " - " << blackPieces[temp].placeOnBoard_Vert<< ","  << blackPieces[temp].placeOnBoard_Horz << "\n";
		
}

double checkForPieces(struct pieces blackPiece) {
	int vert = blackPiece.placeOnBoard_Vert;
	int horz = blackPiece.placeOnBoard_Horz;


	if (vert == 0) {
		return -1; //at the end of the board
	}


	else if (horz + 1 <= 7 && horz - 1 >= 0) { //in the middle of the board
		int rightCheck = checkRight(vert, horz);
		int leftCheck = checkLeft(vert, horz);

		if (leftCheck == 3 && rightCheck != 2) {
			return 3; //enemy on the left
		}
		else if (rightCheck == 2 && leftCheck != 1 ) {
			return 2; //enemy on the right
		}
		else if (rightCheck == 0 && leftCheck == 0) {
			return 0; //surrounded by friendlies
		}
		else if (rightCheck == 2 && leftCheck == 3) {
			return 4; //surrounded by enemies
		}

		return 1.3; //can move either right or left
	}

	else if (horz - 1 >= 0) { //right edge
		return checkLeft(vert, horz);
	}

	else if (horz + 1 <= 7){ //left edge
		return checkRight(vert, horz);
	}

	return -2;

}

double checkLeft(int vert, int horz) {
	if (place[vert - 1][horz - 1].colorOnSpace == 'r') {
		if (place[vert - 1][horz - 1].edge)
			return 0; //can't move
		return 3;//red piece on the left
	}
	if (place[vert - 1][horz - 1].colorOnSpace == 'b') {
		return 0;//black piece on the left
	}

	return 1.2; //no pieces on the left
}

double checkRight(int vert, int horz) {
	if (place[vert - 1][horz + 1].colorOnSpace == 'r') {
		if (place[vert - 1][horz + 1].edge)
			return 0; //can't move
		return 2;//red piece on the left
	}
	if (place[vert - 1][horz + 1].colorOnSpace == 'b') {
		return 0;//black piece on the left
	}

	return 1.1; //no pieces on the right

}

int chooseAPieceForMoving(double moveType[12]) {
	int highestPointValue = -10;
	for (int i = 0; i<12; i++)
	{
		if (floor(moveType[i]) >= highestPointValue) {
			if (floor(moveType[i]) == highestPointValue) {
				int flip = rand() % 2;
				if (flip == 1)
					highestPointValue = i;
			}
			else
				highestPointValue = i;
		}
	}

	return highestPointValue;
}

void computerMove(double moveType, struct pieces blackPiece) {

	std::cout << "move piece - " << blackPiece.placeOnBoard_Vert << "," << blackPiece.placeOnBoard_Horz << "\n\n";
	if (moveType == 1.1) {
		//std::cout << "move right";
		blackPiece.placeOnBoard_Vert = blackPiece.placeOnBoard_Vert -1;
		blackPiece.placeOnBoard_Horz = blackPiece.placeOnBoard_Horz +1;
		std::cout << "move piece - " << blackPiece.placeOnBoard_Vert << "," << blackPiece.placeOnBoard_Horz << "\n\n";
	}
	else if (moveType == 1.2) {
		//std::cout << "move left";
		blackPiece.placeOnBoard_Vert = blackPiece.placeOnBoard_Vert -1;
		blackPiece.placeOnBoard_Horz = blackPiece.placeOnBoard_Horz -1;
		std::cout << "move piece - " << blackPiece.placeOnBoard_Vert << "," << blackPiece.placeOnBoard_Horz << "\n\n";
	}
	else if (moveType == 1.3) {
		//std::cout << "move either";
		int flip = rand() % 2;
		if (flip == 1)
		{
			blackPiece.placeOnBoard_Vert = blackPiece.placeOnBoard_Vert - 1;
			blackPiece.placeOnBoard_Horz = blackPiece.placeOnBoard_Horz + 1;
			std::cout << "move piece - " << blackPiece.placeOnBoard_Vert << "," << blackPiece.placeOnBoard_Horz << "\n\n";
		}
		else {
			blackPiece.placeOnBoard_Vert = blackPiece.placeOnBoard_Vert - 1;
			blackPiece.placeOnBoard_Horz = blackPiece.placeOnBoard_Horz -1;

			std::cout << "move piece - " << blackPiece.placeOnBoard_Vert << "," << blackPiece.placeOnBoard_Horz << "\n\n";
		}

	}
	else if (moveType == 2) {
		//std::cout << "attack right";

	}
	else if (moveType == 3) {
		//std::cout << "attack left";

	}
	else if (moveType == 4) {
		//std::cout << "attack either";
		int flip = rand() % 2;
		if (flip == 1)
		{

		}
	}
	
}


