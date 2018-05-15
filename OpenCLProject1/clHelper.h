#pragma once

#ifndef H_CLHELPER
#define H_CLHELPER


#include <stdio.h>
#include <CL\cl.h>


char* checkError(cl_int error);

void printStatus(char * op, cl_int error);


#endif // !H_CLHELPER
