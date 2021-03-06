
#include "StatReader.h"




statReader::statReader()
{
   statFile.textOpenFile(SKILL_FILE_LOCATION + FILE_LEARNED_SKILLS, false);
   std::string readInput;
   while(statFile.readLine(&readInput)){
      parseInput(readInput);
   }

}

statReader::~statReader()
{
   //nothing
}



std::string statReader::intToStat(int input)
{
   std::string returnStat = "";
   try
   {
      returnStat = statList.at(input);
   }
      catch (const std::out_of_range& oor)
   {
      stringstream ss; ss << "intToStat() failed to find " << input << ".";
      eventLogger -> addNewLog(ss.str());
      returnStat = -1;
   }
   return returnStat;
}

//Finds the value that corresponds to the inputted string.
int statReader::statToInt(std::string input)
{
   int returnInt = -1;
   try
   {
      returnInt = statMap.at(input);
   }
      catch (const std::out_of_range& oor)
   {
      eventLogger -> addNewLog("statToInt() failed to find " + input + ".");
      returnInt = -1;
   }
   return returnInt;
}



//This function reads lines. If the line is not empty and is not a comment (denoted by starting the line with '#'),
//It will create a listing within the statMap, and a corresponding entry in the statList.
bool statReader::parseInput(std::string input)
{
   input = stringModder.trimWhitespace(input, " \t");
   if (input != std::string("") && input.at(0) != '#'){

      try
      {
         statMap.at(input);
      }
         catch (const std::out_of_range& oor)
      {
         statList.push_back(input);
         statMap.insert(std::pair<std::string, int>(input, statList.size()-1));
         return true;
      }
      eventLogger -> addNewLog("Warning: Input: " + input + " already exists in the system.");
   }
   return false;
}

std::vector <std::string> statReader::testing()
{
   return statList;
}






















