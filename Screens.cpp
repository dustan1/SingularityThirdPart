

#include "Screens.h"




ScreenType::ScreenType(std::string newScreenName, FileIO* screenInput)
{
//botX = minX; botY = minY; topX = maxX; topY = maxY;
//Get these from the parsed file...

screenName = newScreenName;
isGood = false;

parseScreenFile(screenInput);
}

ScreenType::~ScreenType()
{


}


//Screens are always stored in the data folder.
//However, the FileIO object passed has all the necessary information.
void ScreenType::parseScreenFile(FileIO* file)
{
   std::string tempInput;
   std::vector <std::string> parserOutput;

   //file -> textOpenFile(screenFileName, false);
   //No longer needed.

   stringFunc stringParcer;

   if (file -> readLine(&tempInput)){
      parserOutput = stringParcer.parseAllTokens(tempInput, "\\");
   }
   else
   {
      //Wut? Broken
   }

   botX = atoi(parserOutput.at(0).c_str()); botY = atoi(parserOutput.at(1).c_str());
   topX = atoi(parserOutput.at(2).c_str()); topY = atoi(parserOutput.at(3).c_str());

   screenID = parserOutput.at(4);


while (file -> readLine(&tempInput)){
   //Empty lines are skipped.
   if (tempInput.empty()){
      continue;
   }

   //Lines with a pound at the beginning are ignored, these are comments.
   if (tempInput.at(0) == '#'){
      //cout << "Read comment: " << tempInput << endl;
      continue;
   }


   if (createInstruction(tempInput)){
      //cout << tempInput << endl;
      continue;
   }

//Something bad happened.
}

isGood = true;
file -> closeFile();
}





bool ScreenType::createInstruction(std::string instructionText)
{
   stringFunc stringParse;
   std::string tokenStr;
   tokenStr = stringParse.parseFirstToken(instructionText, "\\");
   //Using '\' for parsing. Might need to change it later.
   bool returnBool;

   //Integers
   if (tokenStr == "INT"){
      screenInstruct <int>* tempInstruct = new screenInstruct <int>;

      returnBool = tempInstruct -> parseInput(instructionText);
      //debugCodes.push_back(instructionText);
      if (returnBool){
         instructionsInt.push_back(tempInstruct);
         return returnBool;
      }

      //Nothing happened. Why?
      delete tempInstruct;
      return returnBool;
   }

   //Shorts
   if (tokenStr == "ORT"){
      screenInstruct <short>* tempInstruct = new screenInstruct <short>;

      returnBool = tempInstruct -> parseInput(instructionText);
      //debugCodes.push_back(instructionText);
      if (returnBool){
         instructionsShortInt.push_back(tempInstruct);
         return returnBool;
      }

      //Nothing happened. Why?
      delete tempInstruct;
      return returnBool;
   }

   //Just your everyday strings...
   if (tokenStr == "STR"){
      screenInstruct <std::string>* tempInstruct = new screenInstruct <std::string>;

      returnBool = tempInstruct -> parseInput(instructionText);
      //debugCodes.push_back(instructionText);
      if (returnBool){
         instructionsString.push_back(tempInstruct);
         return returnBool;
      }

      //Nothing happened. Why?
      delete tempInstruct;
      return returnBool;
   }

   //Regular doubles. They are formatted differently on output. See screenController.h "parseInstruct(...)"
   if (tokenStr == "DBL"){
      screenInstruct <double>* tempInstruct = new screenInstruct <double>;

      returnBool = tempInstruct -> parseInput(instructionText);
      //debugCodes.push_back(instructionText);
      if (returnBool){
         instructionsDouble.push_back(tempInstruct);
         return returnBool;
      }

      //Nothing happened. Why?
      delete tempInstruct;
      return returnBool;
   }

   //Percent, for formatted output. 99.9%, <0.01%, etc.
   if (tokenStr == "PCT"){
      screenInstruct <double>* tempInstruct = new screenInstruct <double>;

      returnBool = tempInstruct -> parseInput(instructionText);
      //debugCodes.push_back(instructionText);
      if (returnBool){
         instructionsPercentDouble.push_back(tempInstruct);
         return returnBool;
      }

      //Nothing happened. Why?
      delete tempInstruct;
      return returnBool;
   }

   //Null, there is no additional information.
   if (tokenStr == "NUL"){
      screenInstruct <void>* tempInstruct = new screenInstruct <void>;
      returnBool = tempInstruct -> parseInput(instructionText);
      //debugCodes.push_back(instructionText);
      if (returnBool){
         instructionsVoid.push_back(tempInstruct);
         return returnBool;
      }

      delete tempInstruct;
      return returnBool;
   }

   //Not a valid instruction.
   if (!(tokenStr == "INT" || tokenStr == "STR" || tokenStr != "DBL" || tokenStr != "PCT" || tokenStr != "NUL" || tokenStr != "ORT")){
      //debugCodes.push_back("Failed to parse input!");
      return false;
   }
}


//The inputTypes are:
// 'v' for void (No additional data), 'i' for int, 'o' for short, 'p' for percent, 'd' for double, and 's' for string.
std::string ScreenType::parseInstruct(std::string input, char inputType, ...)
{
   //Void / NULL
   if (inputType == 'v'){
      return input;
   }

   va_list args;
   va_start(args, inputType);
   std::stringstream ss(stringstream::in | stringstream::out);

   stringFunc stringTest; //What do I plan to use for the delimiter?
   //'@'? It is not used much in normal writing, so...

   //Breaks the input into components, separated by the '@' character.
   //First finds the location of the '@'

   size_t delim = input.find('@', 0);

   std::string firstString = input.substr(0, delim);
   std::string secondString = input.substr(delim + 1) ;

   ss << firstString;

   //Int
   if (inputType == 'i'){
      int* inputVar = va_arg(args, int*);
      ss << *inputVar;
   }

   //Short. 's' was taken by string.
   else if (inputType == 'o'){
      short* inputVar = va_arg(args, short*);
      ss << *inputVar;
   }

   //Percent output, takes doubles.
   else if (inputType == 'p'){
      double* inputVar = va_arg(args, double*);

      double testDouble = *inputVar;

      if (testDouble < 0){
         testDouble = testDouble * -1;
      }

      if (testDouble < 0.0001){
         ss << "0%";
         //Basically 0.
      }
      else if (testDouble < 0.001){
         ss << "<0.1%";
         //Very unlikely, but can happen. Assumes less than 0.1%, but greater than 0.01%
      }
      else if (testDouble < 0.1){
         //Conversion to percent. Two decimals. "9.98%".
         ss << std::setprecision(2) << fixed ;
         ss << (*inputVar * 100)  << '%';
      }
      else {
         //Conversion to percent. One decimal. "64.8%".
         ss << std::setprecision(1) << fixed ;
         ss << (*inputVar * 100)  << '%';
      }
   }

   //Double
   //Always has 6 characters or less, including the dot and minus sign.
   //Has three modes, tiny, small, and large
   else if (inputType == 'd'){
      double* inputVar = va_arg(args, double*);

      double testDouble = *inputVar;

      //Tiny
      if (*inputVar < 1.00){
         ss << std::setprecision(3) << fixed;
         ss << *inputVar;
      }
      //Small
      else if (*inputVar < 10000.0){
         ss << std::setprecision(4) << fixed;
         ss << *inputVar;
      }
      //Large
      else
      {
         ss.unsetf(ios_base::floatfield);
         ss << std::setprecision(5);
         ss << *inputVar;
      }

   }

   //String
   else if (inputType == 's'){
      std::string* inputVar = va_arg(args, std::string*);
      ss << *inputVar;
   }

   //Default return, this shouldn't happen, but is not a crash worthy error.
   else {
      return std::string();//empty string, not good.
   }

   //Completes the instruction.
   ss << secondString;
   return ss.str();
}



screenOutputData* ScreenType::redraw()
{

   screenOutputData* tempData = new screenOutputData;
   tempData -> screenData.reserve(instructionsInt.size() +
                                  instructionsString.size() +
                                  instructionsDouble.size() +
                                  instructionsShortInt.size() +
                                  instructionsPercentDouble.size() +
                                  instructionsVoid.size() +
                                  10
                                  );

   tempData -> smallX = botX;
   tempData -> smallY = botY;
   tempData -> bigX = topX;
   tempData -> bigY = topY;

   //Initializes the vector to be able to contain all data which should fit into the vector.
   tempData -> screenData.resize(topY - botY + 1);

   //stringFunc stringModder;

   for (int i = 0; i < instructionsInt.size(); i++ ){
      cleanedInstruct tempCleanInstruct;
      tempCleanInstruct.output = parseInstruct(
                        instructionsInt[i] -> instructionText, 'i', instructionsInt[i] -> dataOut);
      tempCleanInstruct.xPos = instructionsInt[i] -> Xpos;
      tempCleanInstruct.yPos = instructionsInt[i] -> Ypos;
      tempData -> screenData.push_back(tempCleanInstruct);
   }


   for (int i = 0; i < instructionsString.size(); i++ ){
      cleanedInstruct tempCleanInstruct;
      tempCleanInstruct.output = parseInstruct(
                        instructionsString[i] -> instructionText, 's', instructionsString[i] -> dataOut);
      tempCleanInstruct.xPos = instructionsString[i] -> Xpos;
      tempCleanInstruct.yPos = instructionsString[i] -> Ypos;
      tempData -> screenData.push_back(tempCleanInstruct);
   }


   for (int i = 0; i < instructionsDouble.size(); i++ ){
      cleanedInstruct tempCleanInstruct;
      tempCleanInstruct.output = parseInstruct(
                        instructionsDouble[i] -> instructionText, 'd', instructionsDouble[i] -> dataOut);
      tempCleanInstruct.xPos = instructionsDouble[i] -> Xpos;
      tempCleanInstruct.yPos = instructionsDouble[i] -> Ypos;
      tempData -> screenData.push_back(tempCleanInstruct);
   }


   for (int i = 0; i < instructionsShortInt.size(); i++ ){
      cleanedInstruct tempCleanInstruct;
      tempCleanInstruct.output = parseInstruct(
                        instructionsShortInt[i] -> instructionText, 'o', instructionsShortInt[i] -> dataOut);
      tempCleanInstruct.xPos = instructionsShortInt[i] -> Xpos;
      tempCleanInstruct.yPos = instructionsShortInt[i] -> Ypos;
      tempData -> screenData.push_back(tempCleanInstruct);
   }


   for (int i = 0; i < instructionsPercentDouble.size(); i++ ){
      cleanedInstruct tempCleanInstruct;
      tempCleanInstruct.output = parseInstruct(
                        instructionsPercentDouble[i] -> instructionText, 'p', instructionsPercentDouble[i] -> dataOut);
      tempCleanInstruct.xPos = instructionsPercentDouble[i] -> Xpos;
      tempCleanInstruct.yPos = instructionsPercentDouble[i] -> Ypos;
      tempData -> screenData.push_back(tempCleanInstruct);
   }


   for (int i = 0; i < instructionsVoid.size(); i++ ){
      cleanedInstruct tempCleanInstruct;
      tempCleanInstruct.output = instructionsVoid[i] -> instructionText;
      tempCleanInstruct.xPos = instructionsVoid[i] -> Xpos;
      tempCleanInstruct.yPos = instructionsVoid[i] -> Ypos;
      tempData -> screenData.push_back(tempCleanInstruct);
   }


return tempData;
}
















