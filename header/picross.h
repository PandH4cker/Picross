#ifndef PICROSS_H
#define PICROSS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>
#include <sox.h>
#include <string.h>

void initArray(int nbLin, int nbCol, int iArray[nbLin][nbCol], int value);
void initArray1D(int size,int iArray[size], int value); 
void ncurses_initialiser();
void ncurses_couleurs();
void draw_grid(int nbLin,int nbCol,int iArray[nbLin][nbCol],int iArrayEx[nbLin][nbCol]);
int nbSeqLines(int nbLin,int nbCol,int iArray[nbLin][nbCol],int index);
int nbSeqCols(int nbLin,int nbCol,int iArray[nbLin][nbCol],int index);
void stackInLines(int nbLin,int nbCol,int iArray[nbLin][nbCol],int size,int stack[size], int index);
void stackInCols(int nbLin,int nbCol,int iArray[nbLin][nbCol],int size,int stack[size], int index);  
char LANCEMENT_JEU(int nbLin,int nbCol,int iArray[nbLin][nbCol]);
int click_souris(int nbLin, int nbCol);
void ncurses_souris();
bool is_OKLines(int nbLin,int nbCol,int iArray[nbLin][nbCol],int iArrayEx[nbLin][nbCol],int eltLin);
bool is_OKCols(int nbLin,int nbCol,int iArray[nbLin][nbCol],int iArrayEx[nbLin][nbCol],int eltCol);
bool hasWin(int nbLin,int nbCol,int iArray[nbLin][nbCol],int iArrayEx[nbLin][nbCol]);
void showArray(int nbLin, int nbCol, int iArray[nbLin][nbCol]);
void loadArrayRec(int nbLin,int nbCol,int iArray[nbLin][nbCol],int acci,int accj);
void loadArray(int nbLin,int nbCol,int iArray[nbLin][nbCol]);
char printWin();
int getIntegerGreaterThanC(int treshold);
int getIntegerGreaterThanL(int treshold);
void loadFromFile(const char * File,int nbLin,int nbCol,int iArray[nbLin][nbCol]);
int choiceMenu(WINDOW * win_in, WINDOW * win);
void printMenu(WINDOW * win,int y,int x);
const char * choicePic(WINDOW * win);

#endif
