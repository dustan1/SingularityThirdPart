#include "ScreenController.h"
#include "eventLog.h"

//variablePointerPointer is a pointer to the dataOut pointer
bool ScreenControl::popFirstUnsetInstructPointer(instructDataOut** output)
{
   if (UnsetPointerList.size() > 0){
      instructDataOut* tempData = new instructDataOut;
      *tempData = UnsetPointerList.front();
      UnsetPointerList.pop_front();
      *output = tempData;
      return true;
   }

   return false;
}


std::vector <instructDataOut> ScreenControl::getAllUnsetInstruct()
{
   std::vector <instructDataOut> tempVector;

   for (std::list <instructDataOut>::iterator it = UnsetPointerList.begin(); it != UnsetPointerList.end(); ++it){
      tempVector.push_back(*it);
   }
UnsetPointerList.clear();
return tempVector;
}



int ScreenControl::makeNewScreen (std::string screenName, std::string screenLocation)
{
   FileIO inputFile;
   std::string filePath = DATA_FOLDER + screenLocation + "/" + screenName + ".sc";
   inputFile.textOpenFile(filePath, false);

   if (! inputFile.isOpen){
      return -1;
   }

   ScreenType* tempScreen = new ScreenType(screenName, &inputFile);

   if (!tempScreen -> isGood){
      return -1;
   }
   screenArray.push_back(tempScreen);

   for (int i = 0; i< tempScreen -> debugCodes.size(); i++){
      testVector.push_back(tempScreen -> debugCodes[i]);
   }


//Things to list of stuff
//{

   for (int i = 0; i < tempScreen -> instructionsString.size(); i++){
      instructDataOut tempInstruct;
      tempScreen -> instructionsString[i] -> dataOutGet = true;

      tempInstruct.macroInstruct = tempScreen -> instructionsString[i] -> dataOutName;
      tempInstruct.variablePointerPointer = (void*) &(tempScreen -> instructionsString[i] -> dataOut);

      UnsetPointerList.push_back(tempInstruct);
   }

   for (int i = 0; i < tempScreen -> instructionsInt.size(); i++){
      instructDataOut tempInstruct;
      tempScreen -> instructionsInt[i] -> dataOutGet = true;

      tempInstruct.macroInstruct = tempScreen -> instructionsInt[i] -> dataOutName;
      tempInstruct.variablePointerPointer = (void*) &(tempScreen -> instructionsInt[i] -> dataOut);

      UnsetPointerList.push_back(tempInstruct);
   }

   for (int i = 0; i < tempScreen -> instructionsDouble.size(); i++){
      instructDataOut tempInstruct;
      tempScreen -> instructionsDouble[i] -> dataOutGet = true;

      tempInstruct.macroInstruct = tempScreen -> instructionsDouble[i] -> dataOutName;
      tempInstruct.variablePointerPointer = (void*) &(tempScreen -> instructionsDouble[i] -> dataOut);

      UnsetPointerList.push_back(tempInstruct);
   }

   for (int i = 0; i < tempScreen -> instructionsShortInt.size(); i++){
      instructDataOut tempInstruct;
      tempScreen -> instructionsShortInt[i] -> dataOutGet = true;

      tempInstruct.macroInstruct = tempScreen -> instructionsShortInt[i] -> dataOutName;
      tempInstruct.variablePointerPointer = (void*) &(tempScreen -> instructionsShortInt[i] -> dataOut);

      UnsetPointerList.push_back(tempInstruct);
   }

   for (int i = 0; i < tempScreen -> instructionsPercentDouble.size(); i++){
      instructDataOut tempInstruct;
      tempScreen -> instructionsPercentDouble[i] -> dataOutGet = true;

      tempInstruct.macroInstruct = tempScreen -> instructionsPercentDouble[i] -> dataOutName;
      tempInstruct.variablePointerPointer = (void*) &(tempScreen -> instructionsPercentDouble[i] -> dataOut);

      UnsetPointerList.push_back(tempInstruct);
   }

//}

   eventLogger -> addNewLog("Adding new screen: " + screenName);

return screenArray.size() - 1;//Good! I think...
}




int ScreenControl::getScreenIndex(std::string screenNameSearch)
{
   for (int i = 0; i < screenArray.size(); i++){
      if (screenArray[i] -> screenName == screenNameSearch){
         return i;
      }
   }
   return -1;
}



bool ScreenControl::outputScreen(int screenIndex)
{

   //If the input cannot be in the array, return false.
   if (screenArray.size() <= screenIndex || screenIndex < 0){
      return false;
   }
   //Call ScreenType::redraw(), then format output.

   screenOutputData* temp = screenArray[screenIndex] -> redraw();

   //Temporary output code;
   outputFormattedLines(temp);

return true;
}

void ScreenControl::outputFormattedLines (screenOutputData* temp)
{
   ConsoleOptions newConsoleOptions;
   stringFunc stringFunctions;

   int counter = (temp -> screenData.size());
   for (int i = 0; i < counter; i++){

      std::vector <std::string> tempStrings = stringFunctions.parseAllTokens(temp -> screenData[i].output, "^");

      newConsoleOptions.cursorOptions.changeCursorPos(temp -> smallX + temp -> screenData[i].xPos,
                                                        temp -> smallY + temp -> screenData[i].yPos);
      newConsoleOptions.cursorOptions.setColour( defaultTextColour, defaultBackColour);


      for (int j = 0; j < tempStrings.size(); j++){
         if (tempStrings[j].size() <= 2){
            continue;
         }

         if ( temp -> screenData[i].output[0] != '^' && j == 0){
            cout << tempStrings[j];
            testVector.push_back(tempStrings[j]);
            continue;
         }
         newConsoleOptions.cursorOptions.setColour( newConsoleOptions.cursorOptions.letterCodeToColourInt(tempStrings[j].at(0)),
                                       newConsoleOptions.cursorOptions.letterCodeToColourInt(tempStrings[j].at(1)));
         cout << tempStrings[j].substr (2);
         //Do colour stuff, then output.
         testVector.push_back(tempStrings[j].substr (2));

      }
      newConsoleOptions.cursorOptions.setColour( defaultTextColour, defaultBackColour);
   }
   newConsoleOptions.cursorOptions.setColour( defaultTextColour, defaultBackColour);
}
//Everything is done!



















