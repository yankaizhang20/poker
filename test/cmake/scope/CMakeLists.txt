set(var 2)

function(test)
    message(STATUS "var in test: ${var}")
endfunction()

test()