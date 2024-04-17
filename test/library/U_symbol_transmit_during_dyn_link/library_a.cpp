//
// Created by zyk on 24-4-11.
//

#include "./api_a.h"


#ifdef TEST
void NoDefFuncA();
#else
void NoDefFuncA()
{
}
#endif

void ApiAnotherFuncA(int)
{
    NoDefFuncA();
}

void ApiFuncA(int)
{
}