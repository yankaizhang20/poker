#!/bin/bash

# @brief: 定义一下工具

# 执行命令，若失败，则终止后续运行
function safe()
{
  command=$1; shift
  ${command} $* || { exit 1; }
}

function test_safe(){

function test_func()
{
    echo $1 $2
    return 0
}

safe test_func var1 var2

echo "last"
}