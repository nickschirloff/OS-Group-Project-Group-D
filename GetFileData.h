//
// Group: Group D
// Author: Corey Hockersmith
// Email: cohocke@okstate.edu
// Date: 9/17/22
// Description: 
//

#ifndef GROUP_GETFILEDATA_H
#define GROUP_GETFILEDATA_H
typedef struct{
    char*** fileArray;
    int fileRows;
    int fileColumns;
    int numberOfUniques;
    char** uniqueArray;
    char* fileName;
    char* columnName;
    int targetColumnIndex;
} fileStruct;
fileStruct GetFileData(char* fileName, char* columnName);
#endif //GROUP_GETFILEDATA_H
