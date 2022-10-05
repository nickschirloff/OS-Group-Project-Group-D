//
// Created by coreytv on 10/4/22.
//

#ifndef CODE_FORKING_H
#define CODE_FORKING_H
typedef struct{
    int** data_pipe; // Still need to implemented by the server code
    int** what_pipe;
} pipeStruct;

void forking(fileStruct fileData, pipeStruct pipeData);
#endif //CODE_FORKING_H
