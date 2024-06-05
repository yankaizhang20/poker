###################################################
# 开发框架。提供模块化开发方式
###################################################

poker_add_library(system DEPENDS pattern type_traits unit)

poker_add_test(system CASE LAUNCH)

poker_add_test(system CASE PROCESS)