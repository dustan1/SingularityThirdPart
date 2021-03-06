#ifndef SCREENCONTROLLER_H_INCLUDED
#define SCREENCONTROLLER_H_INCLUDED

#include "libraryIncluder.h"
#include "Screens.h"
#include "ScreenInstructions.h"

#include <list>

#define DATA_FOLDER "Data/"

#define DEFAULT_COLOUR 15
#define DEFAULT_BACK_COLOUR 0

struct instructDataOut
{
   std::string macroInstruct;
   void* variablePointerPointer;
};



class ScreenControl
{
public:

int defaultTextColour;
int defaultBackColour;


ScreenControl(){
defaultTextColour = DEFAULT_COLOUR;
defaultBackColour = DEFAULT_BACK_COLOUR;

}

//Macros : Stuff and stuff {THIS_IS_A_MACRO} more stuff and things.
//have already been replaced by the instruct input function.
//So the above would be Stuff and stuff @ more stuff and things.
bool popFirstUnsetInstructPointer(instructDataOut** output);
std::vector <instructDataOut> getAllUnsetInstruct();

int makeNewScreen (std::string screenName, std::string screenLocation);

int getScreenIndex(std::string screenNameSearch);

bool outputScreen(int screenIndex);

void outputFormattedLines (screenOutputData* temp);

std::vector <std::string> testVector;

private:

FileIO fileReader;

std::vector < ScreenType* > screenArray;

std::list < instructDataOut > UnsetPointerList;

//Need something to keep track of screens in use...




};





#endif // SCREENCONTROLLER_H_INCLUDED
