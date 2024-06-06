poker_add_library(databus-test DEPENDS databus system)

poker_add_test(databus-test CASE this_process
        DEPENDS
        databus-this_process
)
poker_add_test(databus-test CASE http
        DEPENDS
        databus-http
)