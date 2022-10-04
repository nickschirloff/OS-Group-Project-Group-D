//
// Group: Group D
// Author: Corey Hockersmith
// Email: cohocke@okstate.edu
// Date: 9/17/22
// Description: 
//

#include "GetFileData.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

fileStruct GetFileData(char *fileName, char *columnName){
    columnName=strtok(columnName, "\n");
    int numberOfRows=200;
    int numberOfColumns=0;
    int targetColumn;
    if(strcmp(fileName,"amazonBestsellers.txt")==0){
        numberOfColumns=7;
    }
    else if(strcmp(fileName,"bookInfo.txt")==0){
        numberOfColumns=6;
    }
    int numberOfUniques=0;
    char** uniqueArray= calloc(1, sizeof(char*));
    char*** fileArray=calloc(numberOfRows, sizeof(char**));
    char *buffer= calloc(1024, sizeof(char)); //to temporarily store the line

    FILE *file;
    file=fopen(fileName, "r");
    if (file == NULL){
        printf("Cannot open %s\n", fileName);
        exit(1);
    }
    fgets(buffer, 1024, file);
    for(int row=0; row<numberOfRows; row++){
        fileArray[row]= realloc(fileArray[row], numberOfColumns*sizeof(char*));//allocating column space
    }
    int fileRowNumber=0;
    while(!feof(file)){
        int fileColumnNumber=0;
        char *getLine=strtok(buffer, "\r\n");
        while(getLine != NULL){
            if(fileRowNumber>=numberOfRows){
                numberOfRows+=50;
                fileArray=realloc(fileArray, numberOfRows* sizeof(char**));
                for(int row=numberOfRows-50; row<numberOfRows; row++){
                    fileArray[row]=calloc(numberOfColumns, sizeof(char*));
                }
            }
            if(strchr(getLine, '"')!=NULL){
                char *getTitle=strtok(getLine, "\"");
                getLine=strtok(NULL,  "\"");
                if(fileRowNumber>1&&(strcmp(getTitle, fileArray[fileRowNumber-1][fileColumnNumber])==0)){
                    fileRowNumber--;
                    fileColumnNumber=0;
                    break;
                }

                fileArray[fileRowNumber][fileColumnNumber]=calloc(strlen(getTitle)+1, sizeof(char));
                strcpy(fileArray[fileRowNumber][fileColumnNumber], getTitle);
                //printf("%s,",getTitle); //store this
                fileColumnNumber++;
            } else {
                char *getCommaSeparated=strtok(getLine, ",");
                while(getCommaSeparated!=NULL) {
//    gets rid of duplicates                if((fileRowNumber>1&&fileColumnNumber==0)&&(strcmp(getCommaSeparated, fileArray[fileRowNumber-1][0])==0)){
//                        fileRowNumber--;
//                        fileColumnNumber=0;
//                        continue;
//                    }
                    if((fileRowNumber==0)&&(strcmp(columnName, getCommaSeparated)==0)){
                        targetColumn=fileColumnNumber;
                    }
                    if((targetColumn==fileColumnNumber)&&(fileRowNumber!=0)){
                        if(numberOfUniques==0){
                            uniqueArray[0]=calloc(strlen(getCommaSeparated)+1,sizeof(char));
                            strcpy(uniqueArray[0], getCommaSeparated);
                            numberOfUniques++;
                        }
                        else {
                            int doesExist=-1;
                            for (int uniques = 0; uniques < numberOfUniques; uniques++) {
                                if(strcmp(uniqueArray[uniques], getCommaSeparated)==0){
                                    doesExist=0;
                                }
                            }
                            if(doesExist<0){
                                numberOfUniques++;
                                uniqueArray=realloc(uniqueArray, numberOfUniques*sizeof(char*));
                                uniqueArray[numberOfUniques-1]=calloc(strlen(getCommaSeparated)+1,sizeof(char));
                                strcpy(uniqueArray[numberOfUniques-1], getCommaSeparated);
                            }
                        }
                    }
                    fileArray[fileRowNumber][fileColumnNumber] = calloc(strlen(getCommaSeparated) + 1, sizeof(char));
                    strcpy(fileArray[fileRowNumber][fileColumnNumber], getCommaSeparated);
                    //printf("%s,", getCommaSeparated);//store this
                    getCommaSeparated = strtok(NULL, ",");
                    fileColumnNumber++;
                }
                getLine = strtok(NULL, ",");
            }
        }
        //printf("\n");
        fileRowNumber++;
        fgets(buffer, 1024, file);
        numberOfColumns=fileColumnNumber;
        fileColumnNumber=0;
    }
    //end of file reached
    fclose(file);
    if(fileRowNumber<numberOfRows){
        numberOfRows=fileRowNumber;
        fileArray=realloc(fileArray, numberOfRows*sizeof(char**));
    }

   free(buffer);
//    for(int row=0; row<numberOfRows; row++){
//        for(int column=0; column<numberOfColumns; column++){
//            printf("%-100s", fileArray[row][column]);
//        }
//        printf("\n");
//    }
//    printf("%d\n",numberOfUniques);
//    for(int uniques=0; uniques<numberOfUniques; uniques++){
//        printf("%s\n", uniqueArray[uniques]);
//    }
    fileStruct toReturn;
    toReturn.targetColumnIndex=targetColumn;
    toReturn.columnName=columnName;
    toReturn.fileName=fileName;
    toReturn.fileArray=fileArray;
    toReturn.fileColumns=numberOfColumns;
    toReturn.fileRows=numberOfRows;
    toReturn.uniqueArray=uniqueArray;
    toReturn.numberOfUniques=numberOfUniques;
    return toReturn;
}