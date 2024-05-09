//
// Created by zyk on 24-4-11.
//

#include "./api.h"

#ifdef TEST
void NoDefFunc();
#else
void NoDefFunc()
{
}
#endif

void ApiAnotherFunc(int)
{
    NoDefFunc();
}

void ApiFunc(int)
{
}