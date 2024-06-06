###################################################
# 线程池
###################################################

poker_add_library(thread-pool
        DEPENDS thread-lock thread-condition moodycamel
)

poker_add_test(thread-pool)