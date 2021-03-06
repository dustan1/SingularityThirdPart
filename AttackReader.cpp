
#include "AttackReader.h"
namespace attackScaling
{

//Screens are always stored in the data folder.
//However, the FileIO object passed has all the necessary information.
bool attackReader::readFile(std::string newFileName, std::vector <attackType>* outputVector)
{
   std::string tempInput;
   std::vector <std::string> parserOutput;

   FileIO* file = new FileIO;
   newFileName = ATTACK_DATA_LOCATION + newFileName;
   file -> dataOpenFile(newFileName, false);

   stringFunc stringModder;

   while (true){

      if (file -> readLine(&tempInput) == false){
         eventLogger -> addNewLog("WARNING: Unexpected end of file: " + newFileName);
         break;
      }
      tempInput = stringModder.trimWhitespace(tempInput, " /t"); // Remove leading and trailing spaces.

      if (tempInput.at(0) == '#'){
         continue; //Comment found.
      }

      if (tempInput.substr(0, 9) == "end file;"){
         eventLogger -> addNewLog("INFO: No more attacks to read from file: " + newFileName);
         return true; //End of file.
      }

      outputVector -> push_back( addAttack(file, &stringModder, tempInput) );
   }

   file -> closeFile();
   eventLogger -> addNewLog("INFO: Closing file: " + newFileName);
   return true;
}

attackType attackReader::addAttack(FileIO* file, stringFunc* stringModder, std::string newAttackName)
{
   std::string tempInput;

   attackType tempAttack;
   tempAttack.name = newAttackName;
   //TODO: Make this function.
   //addCostScaling();

   addSubtypeScaling(file, stringModder, &(tempAttack.scaling));
}


bool attackReader::addSubtypeScaling(FileIO* file, stringFunc* stringModder, attackScaling* outputScalings)
{
/*
struct attackScaling
{
   std::vector <statScaling> arcaneScaling;
   std::vector <statScaling> elementalScaling;

   std::vector <statScaling> stabScaling;
   std::vector <statScaling> slashScaling;
   std::vector <statScaling> crushScaling;
   std::vector <statScaling> poisonScaling;
   std::vector <statScaling> DOTScaling;

   std::vector <statScaling> voidScaling;
};
*/

std::vector <std::string> nameOfScalings = {"arcane", "elemental", "stab", "slash", "crush", "poison", "DOT", "void"};
//TODO: get scaling names from file, and make attackScaling dynamic as well.

std::string tempInput;

   while (true){

      if (file -> readLine(&tempInput) == false){
         return false;//No more attacks to read!
      }

      tempInput = stringModder -> trimWhitespace(tempInput, " /t"); // Remove leading and trailing spaces.

      if (tempInput.at(0) == '#'){
         continue; //Comment found.
      }

      for (int i = 0; i < nameOfScalings.size(); i++){
         if (tempInput.substr(0, 9) == nameOfScalings[i] + ';'){
            return addScaling(file, stringModder, &(outputScalings -> Scalings), nameOfScalings[i]);
         }
      }
      //addAttack(FileIO attackFile, stringFunc* stringModder);

   }
}


bool attackReader::addScaling(FileIO* file, stringFunc* stringModder, std::vector <statScaling>* outputScalings, const std::string scalingname)
{

/*
Stats which can scale attacks are:
-Base (Not really a scaling, but must be counted, +x)
-Attacker level (+x per level)
-All base stats (int, etc, +x per stat point)
-Attacker Health and mana (+x per stat point)
-Defender Health and mana (+x per stat point)
-Defender base stats (Much more rare)
-Random number between 0 - x

-All learned skills
   -These are obtained from a file, which maps names to an index.

There are 19 different scalings, in the same order as above.
*/
std::vector <std::string> nameOfScalings = {"arcane", "elemental", "stab", "slash", "crush", "poison", "DOT", "void"};
//TODO: get scaling names from file, and make attackScaling dynamic as well.


statReader statTable;

std::string tempInput;
std::string tempParsedInput;
double scalingValue;

   while (true){

      if (file -> readLine(&tempInput) == false){
         eventLogger -> addNewLog("WARNING: Unexpected end of file in function: addScaling");
         return false;
      }

      tempInput = stringModder -> trimWhitespace(tempInput, " /t"); // Remove leading and trailing spaces.

      if (tempInput.at(0) == '#'){
         continue; //Comment found.
      }

      if (tempInput.substr(0, scalingname.length() + 1) == scalingname + ';'){
         return true;
      }

      for (int i = 0; i < nameOfScalings.size(); i++){
         if (tempInput.substr(0, nameOfScalings[i].length()) == scalingname){
            tempParsedInput = tempInput.substr(nameOfScalings[i].length());
            sscanf(tempParsedInput.c_str(), "%*[^0-9]%lf", &scalingValue);
            statScaling tempStat; tempStat.statNum = i; tempStat.scaling = scalingValue;
            eventLogger -> addNewLog("INFO: added scaling for: " + scalingname);
         }

      }

   }
}


}











