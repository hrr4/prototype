#ifndef FILEPARSER_H
#define FILEPARSER_H

#define LINEBUFSIZE 305
#define VALUESIZE 75

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
  FILE* currentFile;

} FileParser;

int FileParser_OpenFile(char* fileString, FileParser* parserStruct);
int FileParser_CloseFile(FileParser* parserStruct);

long FileParser_FindNodeByString(char* nodeString, FileParser* parserStruct);
long FileParser_FindNodeByCount(char* nodeString, int nodeCount, FileParser* parserStruct);
signed long FileParser_NextNode(long currentPosition, const char* nodeString, FileParser* parserStruct);

int FileParser_NodeCount(const char* nodeString, FileParser* parserStruct);

/*************************************************************************
  RetrieveValue
    - Given an object and tag, return the data attributed to them.

*************************************************************************/
void FileParser_RetrieveValue(long nodePosition, const char* tagString, char* dataOutput, FileParser* parserStruct);
signed int FileParser_RetrieveText(long nodePosition, const int width, const int height, char*** textArray, FileParser* parserStruct);

/*************************************************************************
  Emptylayer
    - Empty the contents, but don't delete the array on the heap, just the memory stored in it.

*************************************************************************/
void FileParser_EmptyLayer(const int width, const int height, char*** textArray);

/*************************************************************************
  Destroy
    - Empty the contents and delete the array from the heap.

    NOTE:
      - Be careful with this!
        : Calling free on memory that is already NULL'd will not be good..

*************************************************************************/
void FileParser_DestroyLayer(const int width, const int height, char*** textArray);

#endif
