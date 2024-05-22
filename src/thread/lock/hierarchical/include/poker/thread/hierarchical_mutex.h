//
// Created by zyk on 24-4-8.
//

#pragma once


namespace poker::thread::lock
{
    /**
     * @brief 具有死锁避免功能的互斥锁
     */
    class hierarchical_mutex;

    void TestHierarchical();
}   // namespace poker::thread::lock