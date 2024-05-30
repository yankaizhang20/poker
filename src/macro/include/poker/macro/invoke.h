//
// Created by zyk on 24-5-30.
//

#pragma once

#include "./concat.h"
#include "./count.h"


#define POKER_INVOKE(func, ...)       POKER_CONCAT(INNER_INVOKE_, POKER_COUNT_ARGS(__VA_ARGS__))(func, __VA_ARGS__)


#define INNER_INVOKE_1(func, a)       func(a)
#define INNER_INVOKE_2(func, a, ...)  func(a) INNER_INVOKE_1(func, __VA_ARGS__)
#define INNER_INVOKE_3(func, a, ...)  func(a) INNER_INVOKE_2(func, __VA_ARGS__)
#define INNER_INVOKE_4(func, a, ...)  func(a) INNER_INVOKE_3(func, __VA_ARGS__)
#define INNER_INVOKE_5(func, a, ...)  func(a) INNER_INVOKE_4(func, __VA_ARGS__)
#define INNER_INVOKE_6(func, a, ...)  func(a) INNER_INVOKE_5(func, __VA_ARGS__)
#define INNER_INVOKE_7(func, a, ...)  func(a) INNER_INVOKE_6(func, __VA_ARGS__)
#define INNER_INVOKE_8(func, a, ...)  func(a) INNER_INVOKE_7(func, __VA_ARGS__)
#define INNER_INVOKE_9(func, a, ...)  func(a) INNER_INVOKE_8(func, __VA_ARGS__)
#define INNER_INVOKE_10(func, a, ...) func(a) INNER_INVOKE_9(func, __VA_ARGS__)
#define INNER_INVOKE_11(func, a, ...) func(a) INNER_INVOKE_10(func, __VA_ARGS__)
#define INNER_INVOKE_12(func, a, ...) func(a) INNER_INVOKE_11(func, __VA_ARGS__)
#define INNER_INVOKE_13(func, a, ...) func(a) INNER_INVOKE_12(func, __VA_ARGS__)
#define INNER_INVOKE_14(func, a, ...) func(a) INNER_INVOKE_13(func, __VA_ARGS__)
#define INNER_INVOKE_15(func, a, ...) func(a) INNER_INVOKE_14(func, __VA_ARGS__)
#define INNER_INVOKE_16(func, a, ...) func(a) INNER_INVOKE_15(func, __VA_ARGS__)
#define INNER_INVOKE_17(func, a, ...) func(a) INNER_INVOKE_16(func, __VA_ARGS__)
#define INNER_INVOKE_18(func, a, ...) func(a) INNER_INVOKE_17(func, __VA_ARGS__)
#define INNER_INVOKE_19(func, a, ...) func(a) INNER_INVOKE_18(func, __VA_ARGS__)
#define INNER_INVOKE_20(func, a, ...) func(a) INNER_INVOKE_19(func, __VA_ARGS__)
#define INNER_INVOKE_21(func, a, ...) func(a) INNER_INVOKE_20(func, __VA_ARGS__)
#define INNER_INVOKE_22(func, a, ...) func(a) INNER_INVOKE_21(func, __VA_ARGS__)
#define INNER_INVOKE_23(func, a, ...) func(a) INNER_INVOKE_22(func, __VA_ARGS__)
#define INNER_INVOKE_24(func, a, ...) func(a) INNER_INVOKE_23(func, __VA_ARGS__)