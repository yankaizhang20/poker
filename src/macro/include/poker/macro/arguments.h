#pragma once

#include "./concat.h"
#include "./count.h"

/**
 * @brief 从参数中，取出指定倒数第i个参数
 */
#define POKER_ARGS_THE_LAST(i, ...) POKER_CONCAT(INNER_ARGS_THE_LAST_, i, _, POKER_COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)


/**
 * @brief 从参数中，去除倒数n个参数
 */
#define POKER_ARGS_EXCEPT_LAST(i, ...) \
    POKER_CONCAT(INNER_ARGS_EXCEPT_LAST_, i, _, POKER_COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

// 取出倒数第1个参数
#define INNER_ARGS_THE_LAST_1_1(x1)                                       x1
#define INNER_ARGS_THE_LAST_1_2(x1, x2)                                   x2
#define INNER_ARGS_THE_LAST_1_3(x1, x2, x3)                               x3
#define INNER_ARGS_THE_LAST_1_4(x1, x2, x3, x4)                           x4
#define INNER_ARGS_THE_LAST_1_5(x1, x2, x3, x4, x5)                       x5
#define INNER_ARGS_THE_LAST_1_6(x1, x2, x3, x4, x5, x6)                   x6
#define INNER_ARGS_THE_LAST_1_7(x1, x2, x3, x4, x5, x6, x7)               x7
#define INNER_ARGS_THE_LAST_1_8(x1, x2, x3, x4, x5, x6, x7, x8)           x8
#define INNER_ARGS_THE_LAST_1_9(x1, x2, x3, x4, x5, x6, x7, x8, x9)       x9
#define INNER_ARGS_THE_LAST_1_10(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) x10

// 取出倒数第2个参数
#define INNER_ARGS_THE_LAST_2_2(x1, x2)                                   x1
#define INNER_ARGS_THE_LAST_2_3(x1, x2, x3)                               x2
#define INNER_ARGS_THE_LAST_2_4(x1, x2, x3, x4)                           x3
#define INNER_ARGS_THE_LAST_2_5(x1, x2, x3, x4, x5)                       x4
#define INNER_ARGS_THE_LAST_2_6(x1, x2, x3, x4, x5, x6)                   x5
#define INNER_ARGS_THE_LAST_2_7(x1, x2, x3, x4, x5, x6, x7)               x6
#define INNER_ARGS_THE_LAST_2_8(x1, x2, x3, x4, x5, x6, x7, x8)           x7
#define INNER_ARGS_THE_LAST_2_9(x1, x2, x3, x4, x5, x6, x7, x8, x9)       x8
#define INNER_ARGS_THE_LAST_2_10(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) x9

// 取出倒数第3个参数
#define INNER_ARGS_THE_LAST_3_3(x1, x2, x3)                               x1
#define INNER_ARGS_THE_LAST_3_4(x1, x2, x3, x4)                           x2
#define INNER_ARGS_THE_LAST_3_5(x1, x2, x3, x4, x5)                       x3
#define INNER_ARGS_THE_LAST_3_6(x1, x2, x3, x4, x5, x6)                   x4
#define INNER_ARGS_THE_LAST_3_7(x1, x2, x3, x4, x5, x6, x7)               x5
#define INNER_ARGS_THE_LAST_3_8(x1, x2, x3, x4, x5, x6, x7, x8)           x6
#define INNER_ARGS_THE_LAST_3_9(x1, x2, x3, x4, x5, x6, x7, x8, x9)       x7
#define INNER_ARGS_THE_LAST_3_10(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) x8

// 除去倒数1个参数
#define INNER_ARGS_EXCEPT_LAST_1_1(x1)
#define INNER_ARGS_EXCEPT_LAST_1_2(x1, x2)                                   x1
#define INNER_ARGS_EXCEPT_LAST_1_3(x1, x2, x3)                               x1, x2
#define INNER_ARGS_EXCEPT_LAST_1_4(x1, x2, x3, x4)                           x1, x2, x3
#define INNER_ARGS_EXCEPT_LAST_1_5(x1, x2, x3, x4, x5)                       x1, x2, x3, x4
#define INNER_ARGS_EXCEPT_LAST_1_6(x1, x2, x3, x4, x5, x6)                   x1, x2, x3, x4, x5
#define INNER_ARGS_EXCEPT_LAST_1_7(x1, x2, x3, x4, x5, x6, x7)               x1, x2, x3, x4, x5, x6
#define INNER_ARGS_EXCEPT_LAST_1_8(x1, x2, x3, x4, x5, x6, x7, x8)           x1, x2, x3, x4, x5, x6, x7
#define INNER_ARGS_EXCEPT_LAST_1_9(x1, x2, x3, x4, x5, x6, x7, x8, x9)       x1, x2, x3, x4, x5, x6, x7, x8
#define INNER_ARGS_EXCEPT_LAST_1_10(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) x1, x2, x3, x4, x5, x6, x7, x8, x9

// 除去倒数2个参数
#define INNER_ARGS_EXCEPT_LAST_2_2(x1, x2)
#define INNER_ARGS_EXCEPT_LAST_2_3(x1, x2, x3)                               x1
#define INNER_ARGS_EXCEPT_LAST_2_4(x1, x2, x3, x4)                           x1, x2
#define INNER_ARGS_EXCEPT_LAST_2_5(x1, x2, x3, x4, x5)                       x1, x2, x3
#define INNER_ARGS_EXCEPT_LAST_2_6(x1, x2, x3, x4, x5, x6)                   x1, x2, x3, x4
#define INNER_ARGS_EXCEPT_LAST_2_7(x1, x2, x3, x4, x5, x6, x7)               x1, x2, x3, x4, x5
#define INNER_ARGS_EXCEPT_LAST_2_8(x1, x2, x3, x4, x5, x6, x7, x8)           x1, x2, x3, x4, x5, x6
#define INNER_ARGS_EXCEPT_LAST_2_9(x1, x2, x3, x4, x5, x6, x7, x8, x9)       x1, x2, x3, x4, x5, x6, x7
#define INNER_ARGS_EXCEPT_LAST_2_10(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) x1, x2, x3, x4, x5, x6, x7, x8

// 除去倒数3个参数
#define INNER_ARGS_EXCEPT_LAST_3_3(x1, x2, x3)
#define INNER_ARGS_EXCEPT_LAST_3_4(x1, x2, x3, x4)                           x1
#define INNER_ARGS_EXCEPT_LAST_3_5(x1, x2, x3, x4, x5)                       x1, x2
#define INNER_ARGS_EXCEPT_LAST_3_6(x1, x2, x3, x4, x5, x6)                   x1, x2, x3
#define INNER_ARGS_EXCEPT_LAST_3_7(x1, x2, x3, x4, x5, x6, x7)               x1, x2, x3, x4
#define INNER_ARGS_EXCEPT_LAST_3_8(x1, x2, x3, x4, x5, x6, x7, x8)           x1, x2, x3, x4, x5
#define INNER_ARGS_EXCEPT_LAST_3_9(x1, x2, x3, x4, x5, x6, x7, x8, x9)       x1, x2, x3, x4, x5, x6
#define INNER_ARGS_EXCEPT_LAST_3_10(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) x1, x2, x3, x4, x5, x6, x7
