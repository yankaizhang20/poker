###################################################
# 循环线程
###################################################

poker_add_library(thread-loop
        DEPENDS unit thread-lock PRIVATE system chrono thread-condition
)