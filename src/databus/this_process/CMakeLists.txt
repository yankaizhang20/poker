###################################################
# databus 进程内实现
###################################################

poker_add_library(databus-this_process
        DEPENDS databus pattern thread-lock thread-pool
)

poker_add_test(databus-this_process)