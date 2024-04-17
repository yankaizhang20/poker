function(fun1)
    func2("arg")
endfunction()

function(func2)
    message(STATUS "arg is " ${ARGN})
endfunction()

fun1()