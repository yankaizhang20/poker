###################################################
# 反射功能
###################################################

poker_add_library(reflect DEPENDS macro type_traits)

poker_add_test(reflect)

poker_add_test(reflect CASE MACRO)