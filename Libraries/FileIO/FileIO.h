
#ifndef FILEIO_H
#define FILEIO_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdarg>
#include <cstdlib>
#include <vector>

#define MAX_BUFFER 4096
#define MAX_DATA_LENGTH 256

//#include "typeChanger.h"

using namespace std;

class FileIO
{

public:
//Variables
fstream myfile;//input and output
std::string filePath;

bool isBinary;
bool isOpen;

unsigned long long fileLength;
bool dataInLineBuffer;

//Functions
inline FileIO();
inline ~FileIO();

inline std::string getFileName();
inline int returnStart();

inline bool getProfile(std::string name);
inline bool makeProfile(std::string name);
inline bool writeDwarf();

//opening files >>>>>

//If isFirstTime is true, the file will be created, closed and then reopened.
//Attempting to use a file without creating it first will not work.
inline int textOpenFile(std::string fileName, bool isFirstTime);
inline int dataOpenFile(std::string filePath, bool isFirstTime);
// <<<<<

//reading data >>>>>
inline int readLine(std::string *output);
inline int readWholeLine(std::string *output);

inline int readIntData(int *output);
inline int readData(int dataLength, int arrayLength, int* errorNum, ...);
// <<<<<

//moving the read/write
inline int goStart(int isRead);
inline int goEnd(int isRead);
inline int goPos(int isRead, int position);

//Writing data
inline int writeDataToFile(const char* data, int length);
inline int writeData(int dataLength, int arrayLength, ...);

inline int writeLine(std::string output);
inline int writeStringText(std::string output);

//Other stuff
std::string getFileName(bool isBinary);
inline int deleteFile(std::string *fileName);
inline bool checkIfOpen();

//Line buffer stuff, to allow for storage in the object. More convenient.
inline void bufferLines(std::string input);
inline void bufferAddition(std::string input);
inline void writeBuffer();
inline void writeBuffer(bool clearData);

inline int clearBuffer(int line);
inline void clearBuffer();

//Data buffer stuff
inline void bufferData(const void* input, int sizeOfData, int arrayLength);

inline int writeBufferData();
inline int writeBufferData(int dummy);

inline void clearDataBuffer();
inline void clearDataBuffer(int dummy);

//Closing the file >>>>>
//The first one checks if the file is open before attempting to close
//The second will, no matter what.
inline int closeFile();
inline void closeFile(bool asdf);
// <<<<<


inline unsigned long long getFileLength();



;
private:

int lineCounter;
int dataCounter;

/*
How the line buffer system works:



*/

vector <std::string> lineBuffer;
std::string lineBufferBuffer;//For extra storage...

vector <char> dataBuffer;

inline void fileConstructor();

};


//Implementation
//{


//Constructor and destructor
//{

FileIO::FileIO()
{
   lineCounter = 0;
   dataCounter = 0;
   isOpen = false;
   dataInLineBuffer = false;
};

FileIO::~FileIO()
{
   closeFile(true);
};

void FileIO::fileConstructor()
{
   lineCounter = 0;
   dataCounter = 0;
   isOpen = true;
   getFileLength();
}

//}


//File deletion
//{

int FileIO::deleteFile(std::string* fileName)
{
   if( remove( (*fileName).c_str() ) != 0 ){
      perror( "Error deleting file" );
      return 0;
   }
   else
   {
      return 1;
   }
}

//}


//Opening files
//{

//Takes a string, and opens the file at that location. Stays at the beginning of the file if not created.
int FileIO::textOpenFile(std::string filePath, bool isFirstTime)
{
   if (isFirstTime){
      myfile.open(filePath.c_str(), ios::out); //creates the file
      myfile.close();
   }

   myfile.open(filePath.c_str(), ios::out | ios::in);

   if (myfile.is_open()){
      isBinary = false;
      fileConstructor();
      return 1;
   }

return 0; //File is not open.
}

//Takes a string, and opens the file at that location.
int FileIO::dataOpenFile(std::string filePath, bool isFirstTime)
{
   if (isFirstTime){
      myfile.open(filePath.c_str(), ios::out);//creates the file
      myfile.close();
      myfile.open(filePath.c_str(), ios::out | ios::in);
      isBinary=true;
      fileLength=0;
      fileConstructor();
      return 1;
   }

   myfile.open(filePath.c_str(), ios::out | ios::in);

   if (myfile.is_open()){
      isBinary=true;
      fileConstructor();
      return 1;
   }

return 0;//file is not open
}

//}



//Closing files
//{

bool FileIO::checkIfOpen()
{
   if (myfile.is_open()){
      isOpen = true;
      return isOpen;
   }

   isOpen = false;
   return isOpen;
}

//Closes the file if and only if there is one open
int FileIO::closeFile()
{
   if (isOpen){
      closeFile(true);
      return 1;
   }
   return 0;

}

//Closes the file, clears the buffer, etc...
void FileIO::closeFile(bool asdf)
{
   lineCounter = 0;
   dataCounter = 0;
   dataInLineBuffer = false;
   myfile.close();
   clearBuffer();
   isOpen = false;
}

//}


//Read lines of text
//{

//Reads one line from the file, and returns it.
int FileIO::readLine(std::string* output)
{
   if (!myfile.good()){
      return 0;
   }

   std::string input;

   //Will read until it hits a line with text.
   while (true){
      getline( myfile, input );
      if (!myfile.good()){
         return 0;
      }
      if (input.length() == 0){
         continue;
      }
      *output = input;
      return 1;
   }
}

//Reads a line from the file, and then appends an endline to it.
int FileIO::readWholeLine(std::string *output)
{
   if (!myfile.good()){
      return 0;
   }

   std::string input;

   getline( myfile, input );
   if (input.length() == 0){
      *output = std::string("\n");
      return 1;
   }
   stringstream ss;
   ss << input << std::endl;
   *output = ss.str();
   return 1;
}

//}


//Text line buffer
//{

//Adds to the line buffer, and then increments the line counter.
void FileIO::bufferLines(std::string input)
{
   if (lineBuffer.empty()){
      lineBuffer.push_back(input);
   }

   if (!dataInLineBuffer){
      lineBuffer.push_back(input);
   }

   lineBufferBuffer += input;

   if (lineBuffer.capacity() <= lineCounter+2){
      lineBuffer.reserve(lineCounter + (lineCounter/2) + 1);
   }

   lineBuffer[lineCounter] = lineBufferBuffer;//push_back(input);[lineCounter]+=input;
   lineCounter++;
   dataInLineBuffer=false;
   lineBufferBuffer.clear();
}

//Writes the buffer to the file, and then clears it.
void FileIO::writeBuffer()
{
   for (int i = 0; i < lineCounter; i++){
      myfile << lineBuffer[i]<<endl;
   }
   if (dataInLineBuffer){
      myfile << lineBufferBuffer;
   }
clearBuffer();
}

//Does not clear the buffer, allowing for multiple calls.
void FileIO::writeBuffer(bool clearData)
{
   for (int i = 0; i < lineCounter; i++){
      myfile << lineBuffer[i] << endl;
   }
   if (dataInLineBuffer){
      myfile << lineBufferBuffer;
   }
}

//Does not increment the counter, and instead adds to the currently stored line.
void FileIO::bufferAddition(std::string input)
{
   //lineBuffer[lineCounter]+=input;
   lineBufferBuffer+=input;
   dataInLineBuffer = true;
}

//Clears the line buffer after line #(line), and sets the counter to that line.
int FileIO::clearBuffer(int line)
{
   if (line > lineCounter||line < 0){
      return 0;
   }

   if (line == 0){
      lineBuffer.clear();
      dataInLineBuffer = false;
      lineBufferBuffer.clear();
      return 1;
   }

   lineBuffer.resize(line);//TODO, is this correct?

   lineBuffer[line].clear();
   lineCounter = line;
   dataInLineBuffer = false;
   lineBufferBuffer.clear();
   if (lineBuffer[line].empty()) {return 1;}
   return 0;
}

//Clears the line buffer after line #(line), and sets the counter to that line.
void FileIO::clearBuffer()
{
   lineBuffer.clear();
   dataInLineBuffer = false;
   lineBufferBuffer.clear();
   lineCounter = 0;

}

//}



//Utilities
//{

//Goes to the end of the file, checks the length, and then goes to the start.
unsigned long long FileIO::getFileLength()
{
   fileLength = 0; unsigned long long returnInt;
   if(!isBinary){//Not a binary file, therefore it is a text file
      goStart(1);
      std::string line;
      while (std::getline(myfile, line)){
         fileLength = fileLength + line.length() + 1;//Length of string plus null terminator
      }
      goStart(1);
   }

   if (isBinary){
      std::streampos fsize = 0;
      goEnd(1);
      fileLength = myfile.tellg() - fsize;
      goStart(1);
   }
returnInt = fileLength; return returnInt;
}

std::string FileIO::getFileName()
{
   char badChars[]={'/', '1', '0', '*', '?', '0', '<', '>', '|'};
   stringstream ss;
   badChars[1]=92; badChars[2]=54; badChars[5]=34;
   //54 = :, 34 = "
   std::string fileName; std::string input;
   int inputLength;
   bool charGood = false;
   int i;int j;

   while (true){
      system("CLS");
      cout<<"Please type in the name of the file. It should be located in the Data folder."<<endl;
      cout<<"If it does not currently exist, it will be created."<<endl;
      getline(cin, input);//Includes spaces - http://cboard.cprogramming.com/cplusplus-programming/122401-how-do-i-allow-spaces-cin.html
      //cin>>input;
      inputLength = input.length();

      if (inputLength<=0||inputLength>=255){
         cout<<"The string is not the correct length."<<endl;
         system("PAUSE");
         continue;
      }
      break;
   }

   //i=0;
   j=0;
   while(true){//for (int i=0; i<inputLength;i++){
      charGood = true;

      for (int k=0;k<9;k++){
         if (input.at(j)==badChars[k]){
            charGood=false;
         }
      }
      if (charGood){
         ss << input.at(j);//fileName.at(i) = input.at(j);
         //i++;
      }
      j++;
      charGood = false;
   }
   fileName = ss.str();
   ss.str(std::string());//clears the stringstream

   if (fileName.at(inputLength-1)=='.'){
      fileName.erase(inputLength - 1);
   }
   ss<<"/Data/"<<fileName;
   filePath = ss.str();

return fileName;
}

//When isRead is 1 or higher, moves the get position. If it is 0, moves the put pointer. Else, returns 0.
int FileIO::goStart(int isRead)
{
   myfile.clear();
   if (isRead > 0){
      myfile.seekg(0, ios::beg);
      return 1;
   }
   if (isRead == 0){
      myfile.seekp(0, ios::beg);
      return 1;
   }

   return 0;
}

int FileIO::goEnd(int isRead)
{
   myfile.clear();
   if (isRead > 0){
      myfile.seekg(0, ios::end);
      return 1;
   }
   if (isRead == 0){
      myfile.seekp(0, ios::end);
      return 1;
   }

   return 0;
}

int FileIO::goPos(int isRead, int position)
{

   if (position<0||position>fileLength){
      return 0;
   }

   if (isRead > 0){
      myfile.seekg(0, ios_base::beg);
      myfile.seekg(position);
      return 1;
   }
   if (isRead == 0){
      myfile.seekp(0, ios_base::beg);
      myfile.seekp(position);
      return 1;
   }

   return 0;

}


//}


// It is limited to MAX_BUFFER bytes per pull.
// Multiple executions may be required to get all data.
// http://stackoverflow.com/questions
// /1579719/variable-number-of-parameters-in-function-in-c
int FileIO::readData(int dataLength, int arrayLength, int* errorNum, ...)
{
//For dataLength,
//this affects how many bytes are taken from the file per arrayLength.

//returns 0 for failure, else 1

   char* buffer= new char [MAX_BUFFER];
   //char * buffer = new char [length];
   //This stores 512 4 byte objects (such as integers), or 256 doubles.
   char* output; //void * temp;
   int bytesToGet=dataLength;
   int totalBytesToGet=0;
   int j = 0;
   *errorNum = 0;
   if (dataLength>=MAX_DATA_LENGTH||dataLength<=-1){
      delete [] buffer;
      return 0;
   }

   va_list ap;
   va_start(ap, errorNum);//Gets the pointer to the function parameter list

   output = va_arg(ap, char*);//This allows for modification, and pointer arithmetic
   va_end(ap);//closes list, important...

   if (output == NULL){
      cout<<"???"<<endl;
      delete [] buffer;
      *errorNum = -1;//Input error
      return 0;
   }


   totalBytesToGet = (bytesToGet * arrayLength);
   if (totalBytesToGet>MAX_BUFFER){
      delete [] buffer;
      *errorNum = -1;//Input error
      return 0;
   }

   //goStart(1);//Temporary
   //read bytes, chance to fail...
   myfile.read(buffer, totalBytesToGet);
   int bytesRead = myfile.gcount();

   if (bytesRead!=totalBytesToGet){
      //goStart(1);//Goes to start, temporary.
      myfile.clear();
      cout<<"asdf"<<endl;
      system("PAUSE");
      delete [] buffer;
      *errorNum = -2;//Read error, not enough bytes to get.
      return -1 * bytesRead;//Bad stuff, note that all bad returns are less than one.
   }

   //if (myfile.eof()&&myfile.fail()) return 0; //hit end of file...

   for (int i=0;i<(arrayLength*bytesToGet);i++){
      *(output+i) = buffer[i];
   }
   //system("PAUSE");
   *errorNum = 1;
   delete [] buffer;
   return 1;

}

//Puts a newline after the output
int FileIO::writeLine(std::string output)
{

   if (myfile.is_open()){
      myfile << output<<endl;
      return 1;
   }

return 0;
}

//Does not put a newline after the output
int FileIO::writeStringText(std::string output)
{
   if (myfile.is_open()){
      myfile << output;
      return 1;
   }
return 0;
}


//Clears entire buffer, and resets counter
void FileIO::clearDataBuffer()
{
   dataInLineBuffer = false;
   dataCounter = 0;
   dataBuffer.clear();
}



//Takes any array (including single value pointers) and writes it to the file.
//Returns the number of indexes written to the file.
//int FileIO::writeData(int dataLength, int arrayLength, ...){

void FileIO::bufferData(const void* input, int sizeOfData, int arrayLength)
{
   int dataSize = sizeOfData * arrayLength; char* charInput = (char*)input;
   for (int i=0; i<dataSize; i++){
      dataBuffer.push_back(*(charInput + i));
   }
}

int FileIO::writeBufferData()
{
int temp = 0;
if (dataBuffer.size()!=0){
   temp = writeData(sizeof(char), dataBuffer.size(), &(dataBuffer[0]));
   if (temp>0){
      clearDataBuffer();
      return temp;
   }
}
return 0;

}

int FileIO::writeBufferData(int dummy)//Does not clear buffer
{
if (dataBuffer.size()!=0){
   return writeData(sizeof(char), dataBuffer.size(), &(dataBuffer[0]));
}
return 0;

}




//Takes data. Writes it. Requires length in bytes of the data.
int FileIO::writeDataToFile(const char* data, int length)
{
   if (myfile.is_open()){
      myfile.write(data, length); return !myfile.fail();
   }
return 0;
}


//Takes any array (including single value pointers) and writes it to the file.
//Returns the number of indexes written to the file.
int FileIO::writeData(int dataLength, int arrayLength, ...){

if (dataLength<=0||arrayLength<=0){
   return 0;
}

va_list ap;
const char* dataBytes;
va_start(ap, arrayLength);

   dataBytes = va_arg(ap, const char*);

va_end(ap);

if (!(writeDataToFile(dataBytes, dataLength * arrayLength))){
   return 0;
}
   dataBytes=dataBytes+dataLength;//I forgot to change the variable. Derp.
   return 1;
//return arrayLength;
}




//}





;;;;;;

#endif







