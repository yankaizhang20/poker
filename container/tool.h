//
// Created by zyk on 23-11-14.
//

#pragma once

namespace {
    /**
     * @brief 将容器内 符合条件 的元素 执行某个动作后 从容器内移除
     */
    template<typename Container, typename Pred, typename Operation>
    inline void do_and_update_if(Container &container, Pred p, Operation op) {
        for (auto itr = container.begin(); itr != container.end();) {
            if (p(*itr)) {
                op(*itr);
                itr = container.erase(itr);
            } else {
                ++itr;
            }
        }
    }
}   // namespace