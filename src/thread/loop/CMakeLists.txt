###################################################
# 循环线程
###################################################

poker_add_library(thread-loop
        DEPENDS unit thread-lock thread-condition PRIVATE system chrono
)