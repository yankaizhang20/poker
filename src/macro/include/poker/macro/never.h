//
// Created by yarten on 2021/10/23.
//

#pragma once

#include <iostream>

#define INNER_RAISE_FATAL_ERROR(tag_string, ...)                      \
    (std::cerr << tag_string << std::string(__VA_ARGS__) << std::endl \
               << "file: " << __FILE__ << std::endl                   \
               << "line: " << __LINE__ << std::endl,                  \
     std::terminate())


/**
 * @brief 标识某种流程不应该发生，若实际发生了，说明程序存在逻辑实现错误
 */
#define poker_never(...) INNER_RAISE_FATAL_ERROR("nox_never: ", __VA_ARGS__)


/**
 * @brief 用于开发阶段，标识某个流程没有完全开发结束，执行本函数将直接退出程序，可设置错误信息
 */
#define poker_no_impl(...) INNER_RAISE_FATAL_ERROR("no implementation error: ", __VA_ARGS__)


/**
 * @brief 标识某个流程一定不会到达，以消除编译器对 no return 的警告；若一旦真的到达，程序会退出。
 */
#define poker_unreachable_code(...) INNER_RAISE_FATAL_ERROR("reach code that should not be reached: ", __VA_ARGS__)
