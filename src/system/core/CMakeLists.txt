###################################################
# 框架核心
###################################################

poker_add_library(core DEPENDS system-launch_center system-auto_start)

poker_add_test(core CASE LAUNCH)

poker_add_test(core CASE PROCESS)