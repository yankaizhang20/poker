###################################################
# 通信 http 实现
###################################################

poker_add_library(databus-http
        DEPENDS databus databus_serialization system param
)

poker_add_test(databus-http)