#include "FileParser.h"

int FileParser_OpenFile(char* fileString, FILEPARSER* parserStruct)
{
  if ((parserStruct->currentFile = fopen(fileString, "rb")) != 0) 
    return 1; 
  return 0;
}

int FileParser_CloseFile(FILEPARSER* parserStruct)
{
  if (fclose(parserStruct->currentFile) == 0)
    return 0;
  return EOF;
}

long FileParser_FindNodeByString(char* nodeString, FILEPARSER* parserStruct) { return FileParser_NextNode(fseek(parserStruct->currentFile, 0, SEEK_SET)-strlen(nodeString), nodeString, parserStruct); }

long FileParser_FindNodeByCount(char* nodeString, int nodeCount, FILEPARSER* parserStruct)
{
  long currentPosition = 0;

  while (nodeCount-- >= 1 && currentPosition != EOF) currentPosition = FileParser_NextNode(currentPosition, nodeString, parserStruct);

  return (currentPosition != EOF) ? FileParser_NextNode(currentPosition, nodeString, parserStruct) : EOF;/* currentPosition; */
}

void FileParser_RetrieveValue(long nodePosition, const char* tagString, char* dataOutput, FILEPARSER* parserStruct)
{
  int tagLength = strlen(tagString);
  char lineBuf[LINEBUFSIZE], *delims = "=\"";

  if (nodePosition >= 0LL)
  {
    fseek(parserStruct->currentFile, nodePosition, SEEK_SET);

    if (fgets(lineBuf, LINEBUFSIZE, parserStruct->currentFile))
    {
      char* currentTag;

      if ((currentTag = strstr(lineBuf, tagString)) != 0)
      {
        /* Increment the substring pointer past the actual tag string. */
        while (tagLength-- > 0) currentTag++;

        strncpy(dataOutput, strtok(currentTag, delims), VALUESIZE);
      }
    }
  }
  else
    strncpy(dataOutput, "", VALUESIZE);
}


int FileParser_RetrieveText_AsInt(long nodePosition, const int width, const int height, int** intArray, FILEPARSER* parserStruct)
{
  int row = 0, col = 0, byteCount;
  void** voidBuf = {0};

  /* Get the text. */
  byteCount = FileParser_RetrieveText(nodePosition, width, height, 4, &voidBuf, parserStruct);

  /* Allocate intArray...it should be unallocated at the moment. */
  *intArray = (int *) malloc(byteCount * sizeof(int*));

  if (!*intArray)
    return -1;

  for (row = 0; row < height; ++row)
    for (col = 0; col < width; ++col)
      (*intArray)[col+(row*height)] = atoi((char*)voidBuf[col+(row*height)]);

  FileParser_DestroyLayer(width, height, &voidBuf);
  
  return 0;
}

int FileParser_RetrieveText_AsString(long nodePosition, const int width, const int height, char*** stringArray, FILEPARSER* parserStruct)
{
  int row = 0, col = 0, byteCount;
  void** voidBuf = {0};

  /* Get the text. */
  byteCount = FileParser_RetrieveText(nodePosition, width, height, 4, &voidBuf, parserStruct);

  *stringArray = (char **) malloc(byteCount * sizeof(char*));

  if (!*stringArray)
    return -1;

  memcpy(**stringArray, voidBuf, byteCount * sizeof(char*));
  FileParser_DestroyLayer(width, height, &voidBuf);
    
  return 0;
}

static int FileParser_RetrieveText(long nodePosition, const int width, const int height, const int positionSize, void*** textArray, FILEPARSER* parserStruct)
{
  char lineBuf[LINEBUFSIZE], *delims = ",\n";

  if (nodePosition >= 0LL)
  {
    fseek(parserStruct->currentFile, nodePosition, SEEK_SET);

    if (fgets(lineBuf, LINEBUFSIZE, parserStruct->currentFile))
    {
      int i = 0, byteCount = 0;
      char nodeName[15], *linePos = lineBuf;
      
      *textArray = (void**) malloc(width * height * positionSize);

      if (!*textArray)
        return EOF;

      /* Get rid of spaces. */
      while (*linePos == ' ') linePos++;

      /* Get past the rest of the tags and get to the textual data! */
      while (sscanf(linePos, "<%s", nodeName) >= 1) 
      {
        fgets(lineBuf, LINEBUFSIZE, parserStruct->currentFile);
        linePos = lineBuf;
        while (*linePos == ' ') linePos++;
      }

      linePos = lineBuf;

      /* Should be at the beginning of the data we need. */
      while (sscanf(linePos, "</%s", nodeName) < 1)
      {
        void* currentToken = strtok(lineBuf, delims);

        while (currentToken != NULL && i < width*height)
        {
          (*textArray)[i] = malloc(positionSize);
          byteCount += positionSize;

          if (!(*textArray)[i])
            return EOF;

          memcpy((*textArray)[i++], currentToken, positionSize);

          currentToken = strtok(NULL, delims);
        }

        fgets(lineBuf, LINEBUFSIZE, parserStruct->currentFile);
        linePos = lineBuf;
        while (*linePos == ' ') linePos++;
      }

      return byteCount;
    }
  }

  return EOF;
}

long FileParser_NextNode(long currentPosition, const char* nodeString, FILEPARSER* parserStruct)
{
  char lineBuf[LINEBUFSIZE], nodeName[15] = { 0 };

  /* Goto the position we're starting from. */
  fseek(parserStruct->currentFile, currentPosition+strlen(nodeString), SEEK_SET);

  while (!feof(parserStruct->currentFile))
  {
    /* Get the rest of the line. */
    if (fgets(lineBuf, LINEBUFSIZE, parserStruct->currentFile))
    {
      char* linePos = lineBuf;

      /* Check for spaces because of XML nesting.
         If we have some, incrememt the pointer. */
      while (*linePos == ' ') linePos++;

      /* Read the new node name. */
      sscanf(linePos, "<%s", nodeName);
    }

    /* See if we're at the right node. */
    if (strcmp(nodeName, nodeString) == 0)
      /* Match */
      return (ftell(parserStruct->currentFile) - strlen(lineBuf));
  }

  return EOF;
}

int FileParser_NodeCount(const char* nodeString, FILEPARSER* parserStruct)
{
  int count = 0;
  long currentPosition = 0;

  while ((currentPosition = FileParser_NextNode(currentPosition, nodeString, parserStruct)) != EOF) count++;

  return count;
}

void FileParser_EmptyLayer(const int width, const int height, void*** textArray)
{
  int row, col;

  for (row = 0; row < height; ++row)
    for (col = 0; col < width; ++col)
      free((*textArray)[col+(row*height)]);
}

void FileParser_DestroyLayer(const int width, const int height, void*** textArray)
{
  FileParser_EmptyLayer(width, height, textArray);

  free(*textArray);
}
