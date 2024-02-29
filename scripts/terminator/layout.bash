#!/bin/bash

# @brief: 创建 layout，启动传入的多个命令
# $n: 任务名：执行目录：命令，如 task1:/bin/:cd
function launch()
{
    # 存放各个任务的配置项
    local task_names=()
    local task_work_directories=()
    local task_commands=()

    # 解析各任务配置
    for task_profile in "$@";do
      echo task_profile="${task_profile}"

      local profile
      while read entry; do
        profile+=("${entry}")
      done < <(echo "${task_profile}" | tr : \\n)

      task_names+=("${profile[0]}")
      task_work_directories+=("${profile[1]}")
      task_commands+=("${profile[2]}")

      profile=()
    done

    echo task_names="${task_names[*]}"
    echo task_work_directory="${task_work_directories[*]}"
    echo "task_command nums=${#task_commands[*]}"

    # 创建 terminator layout
    create_layout "${task_names[@]}" "${task_work_directory[@]}" "${task_commands[@]}"

    # 启动 terminator
}

# @brief: 为指定任务集合创建 terminator layout
# $1: 任务名数组
# $2: 任务工作目录数组
# $3: 任务指令数组
function create_layout()
{
    #
}