//
// Created by zyk on 24-5-9.
//

#pragma once


// 创建唯一名，__COUNTER__ 随调用次数自增
#define POKER_COUNTER_UNIQUE(base) POKER_CONCAT(base, _COUNTER_, __COUNTER__)

// 创建唯一名，根据代码所在行数
#define POKER_LINE_UNIQUE(base) POKER_CONCAT(base, _LINE_, __LINE__)
