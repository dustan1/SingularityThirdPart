
#ifndef SCREENS_H
#define SCREENS_H

#include <fstream>
#include <vector>
//#include <map>
#include <stdarg.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "libraryIncluder.h"

#include "ScreenInstructions.h"



#define PRESSDELAY 5
#define SUCCESS_NUM 1
#define FAILURE_NUM -1
#define FAILURE_NUM_OTHER -2

#define MAX_LINE_LENGTH 510

#define SCREEN_HEIGHT 24
#define SCREEN_WIDTH 80

#define NUM_FUNCTIONS

struct cleanedInstruct {
   std::string output;
   int xPos;
   int yPos;
};

struct screenOutputData {
   int smallX; int smallY;
   int bigX; int bigY;

   std::vector < cleanedInstruct > screenData;
};



using namespace std;



class ScreenType
{


public:
;;
//Name given to screen by caller.
std::string screenName;

//Name given to screen by file.
std::string screenID;

ScreenType(std::string newScreenName, FileIO* screenInput);

~ScreenType();

//This function returns a data structure that tells the parent lines to draw, and where to draw things.
screenOutputData* redraw();


std::vector <screenInstruct <int>* > instructionsInt;
std::vector <screenInstruct <std::string>* > instructionsString;
std::vector <screenInstruct <double>* > instructionsDouble;
std::vector <screenInstruct <short>* > instructionsShortInt;

std::vector <screenInstruct <double>* > instructionsPercentDouble;

std::vector <screenInstruct <void>* > instructionsVoid;

bool isGood;

std::vector <std::string> debugCodes;

private:


;;
int botX; int botY; int topX; int topY;//The screen's area of influence. This is inclusive.


std::string parseInstruct(std::string input, char inputType, ...);

void parseScreenFile(FileIO* file);

bool createInstruction(std::string instructionText);

};

#endif





