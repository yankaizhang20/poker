#!/bin/bash

# @brief: 创建 layout，启动传入的多个命令
# $n: 任务名：执行目录：命令，如 task1:/bin/:cd
function launch()
{
    # 解析各任务配置，并生成创建配置的调用命令
    local create_layout_command="create_layout"

    for task_profile in "$@";do
      echo task_profile="${task_profile}"

      local profile
      while read entry; do
        profile+=("${entry}")
      done <<< $(echo "${task_profile}" | tr : \\n)

      create_layout_command+=" -t ${profile[0]}"
      create_layout_command+=" -d ${profile[1]}"
      create_layout_command+=" -x \"${profile[2]}\""

      profile=()
    done

    # 创建 terminator config 临时文件
    local temp_terminator_config=$(mktemp)

    # TODO: 将 [global_config] [profiles] 写入 config 文件内

    # 调用 create_layout 创建 terminator config TODO:写入临时文件中
    eval ${create_layout_command}

#    if [ $? -eq 0 ]; then
#      # 以创建的 config 文件启动启动 terminator
#      terminator --config ${temp_terminator_config}
#    else
#      echo "创建 terminator config 失败"
#      return -1;
#    fi
}


# @brief: 为指定任务集合创建 terminator layout
# @named: -t 任务名
# @named: -d 任务工作目录
# @named: -x 任务指令
function create_layout()
{
    # 读取任务配置
    local names=()
    local dirs=()
    local commands=()

    while getopts t:d:x: opt;do
      case ${opt} in
      t) names+=("${OPTARG}");;
      d) dirs+=("${OPTARG}");;
      x) commands+=("${OPTARG}");;
      *) echo "错误使用";;
      esac
    done

    shift $((OPTIND -1))

    if [ ${#names[@]} -ne ${#dirs[@]} ] || [ ${#names[@]} -ne ${#commands[@]} ] || [ ${#dirs[@]} -ne ${#commands[@]} ];then
      echo "创建 terminator config: 错误的输入！"
      return 1;
    fi

    # 任务数量
    local task_nums=${#names[@]}

    # 创建 [layout]
}