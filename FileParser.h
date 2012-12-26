#ifndef FILEPARSER_H
#define FILEPARSER_H

#define LINEBUFSIZE 305
#define VALUESIZE 75

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct
{
  FILE* currentFile;

} FILEPARSER;

int FileParser_OpenFile(char* fileString, FILEPARSER* parserStruct);
int FileParser_CloseFile(FILEPARSER* parserStruct);

long FileParser_FindNodeByString(char* nodeString, FILEPARSER* parserStruct);
long FileParser_FindNodeByCount(char* nodeString, int nodeCount, FILEPARSER* parserStruct);
long FileParser_NextNode(long currentPosition, const char* nodeString, FILEPARSER* parserStruct);

int FileParser_NodeCount(const char* nodeString, FILEPARSER* parserStruct);

/*************************************************************************
  RetrieveValue
    - Given an object and tag, return the data attributed to them.

*************************************************************************/
void FileParser_RetrieveValue(long nodePosition, const char* tagString, char* dataOutput, FILEPARSER* parserStruct);

int FileParser_RetrieveText_AsInt(long nodePosition, const int width, const int height, int** intArray, FILEPARSER* parserStruct);
int FileParser_RetrieveText_AsString(long nodePosition, const int width, const int height, char*** stringArray, FILEPARSER* parserStruct);

static int FileParser_RetrieveText(long nodePosition, const int width, const int height, const int positionSize, void*** textArray, FILEPARSER* parserStruct);

/*************************************************************************
  Emptylayer
    - Empty the contents, but don't delete the array on the heap, just the memory stored in it.

*************************************************************************/
void FileParser_EmptyLayer(const int width, const int height, void*** textArray);

/*************************************************************************
  Destroy
    - Empty the contents and delete the array from the heap.

    NOTE:
      - Be careful with this!
        : Calling free on memory that is already NULL'd will not be good..

*************************************************************************/
void FileParser_DestroyLayer(const int width, const int height, void*** textArray);

#endif
