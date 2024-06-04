//
// Created by zyk on 24-3-15.
//

#pragma once

#include "./httplib.h"

// 该宏属于系统宏，并且和 eigen 中的名字产生冲突
#ifdef _res
#    undef _res
#endif