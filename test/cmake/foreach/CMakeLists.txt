cmake_minimum_required(VERSION 3.15)

project(test-foreach)

set(var "")

foreach (entry "${var}")
    message(STATUS hit it)
endforeach ()
