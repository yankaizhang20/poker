//
// Created by zyk on 24-4-11.
//

#include "./api.h"

#ifdef TEST
void Another();
#else
void Another()
{
}
#endif

void ApiFun(int)
{
    Another();
}