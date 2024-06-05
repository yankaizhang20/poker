//
// Created by zyk on 24-6-5.
//

#pragma once

#include <poker/unit.h>


namespace poker::chrono
{
    /**
     * @return 当前的系统时间
     */
    unit::Time now();

    /**
     * @return 当前的时间（单位：秒）
     */
    double sec();

    /**
     * @return 当前的毫秒数
     */
    long long ms();

    /**
     * @return 当前的微秒数
     */
    long long us();

    /**
     * @return 当前的纳秒数
     */
    long long ns();

    /**
     * @return 返回一个日期 + 时间，可用于按时间命名的场景（如，创建日志文件）
     */
    std::string date();
}   // namespace poker::chrono