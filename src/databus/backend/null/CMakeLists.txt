###################################################
# databus 空实现，所有通信将不起作用
###################################################

poker_add_library(databus-null
        DEPENDS databus
)

poker_add_test(databus-null)
