//
// Created by zyk on 24-4-11.
//

#include "./api_b.h"


#ifdef TEST
void NoDefFuncB();
#else
void NoDefFuncB()
{
}
#endif

void ApiAnotherFuncB(int)
{
    NoDefFuncB();
}

void ApiFuncB(int)
{
}