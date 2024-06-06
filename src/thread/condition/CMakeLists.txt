###################################################
# 条件变量
###################################################

poker_add_library(thread-condition DEPENDS unit)

poker_add_test(thread-condition CASE SEMAPHORE
        DEPENDS system
)

poker_add_test(thread-condition CASE COUNTER)