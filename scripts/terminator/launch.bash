#!/bin/bash

# @brief: 创建 layout，从 bash 中启动传入的多个命令
# @position: n 任务名：执行目录：命令，如 task1:/bin/:cd
function launch()
{
    # 解析各任务配置，并生成创建配置的调用命令
    local create_layout_command="create_layout"

    for task_profile in "$@";do
      local profile
      while read entry; do
        profile+=("${entry}")
      done <<< "$(echo "${task_profile}" | tr : \\n)"

      create_layout_command+=" -t ${profile[0]}"
      create_layout_command+=" -d ${profile[1]}"
      create_layout_command+=" -x \"trap /bin/bash 2;${profile[2]}; bash\""

      profile=()
    done

    # 创建 terminator config 临时文件
    local temp_terminator_config
    temp_terminator_config=$(mktemp)

    # 将 [global_config] [profiles] 写入 config 文件内
    local root_script_dir="$(dirname "${BASH_SOURCE[0]}")"
    cat "${root_script_dir}"/config_global_config.template > "${temp_terminator_config}" 2>/dev/null
    cat "${root_script_dir}"/config_profile.template >> "${temp_terminator_config}" 2>/dev/null

#    eval ${create_layout_command}

    # 调用 create_layout 创建 terminator config
    if echo "$(eval ${create_layout_command})" >> "${temp_terminator_config}" 2>/dev/null; then
      # 以创建的 config 文件启动启动 terminator
      terminator --config "${temp_terminator_config}"
    else
      echo "创建 terminator config 失败"
      return 1;
    fi
}


# @brief: 为指定任务集合创建 terminator layout
# @named: -t 任务名
# @named: -d 任务工作目录
# @named: -x 任务指令
# 目标分割节点类型
child_type=VPaned
function create_layout()
{
    ###读取任务配置
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

    #任务数量
    local task_nums=${#names[@]}

    ###创建 layout
    local layout_indent=' '
    local terminal_indent='  '
    local config_entry_indent='   '
    local layout_config="[layouts]"

    #主窗口配置
    layout_config+="
    ${layout_indent}[[default]]
    ${terminal_indent}[[[child0]]]
    ${config_entry_indent}type = Window
    ${config_entry_indent}parent = \"\"
    ${config_entry_indent}order = 0
    ${config_entry_indent}position = 0:0
    ${config_entry_indent}size = 1920, 1030
    "

    ##创建分割节点
    #当前分割节点数量
    local cur_child_nums=0
    #目标分割节点数量
    local target_child_nums=$((task_nums-1))

    while [ ${cur_child_nums} -ne ${target_child_nums} ];do
      ((cur_child_nums++))
      # 父节点名
      local parent=child$((cur_child_nums/2))
      # 位于父节点的左或右（上或下）
      local order=$((cur_child_nums%2))

      # TODO 确认第一个节点的 order 为 1 是否可以
      if [ "${cur_child_nums}" -eq 1 ];then
        order=0
      fi

      layout_config+="${terminal_indent}[[[child${cur_child_nums}]]]
      ${config_entry_indent}type = ${child_type}
      ${config_entry_indent}parent = ${parent}
      ${config_entry_indent}order = ${order}
      ${config_entry_indent}ratio = 0.5
      "
    done

    ##创建任务数量的 terminal 节点
    #当前 terminal 节点数量
    local cur_terminal_nums=0
    #当前 terminal 在节点树中的位置
    local cur_terminal_index=${target_child_nums}

    while [ ${cur_terminal_nums} -ne ${task_nums} ];do
      ((cur_terminal_nums++))
      ((cur_terminal_index++))

      # 父节点名
      local parent=child$((cur_terminal_index/2))
      # 位于父节点的左或右（上或下）
      local order=$((cur_terminal_index%2))

      layout_config+="${terminal_indent}[[[terminal${cur_terminal_nums}]]]
      ${config_entry_indent}type = Terminal
      ${config_entry_indent}profile = default
      ${config_entry_indent}parent = ${parent}
      ${config_entry_indent}order = ${order}
      ${config_entry_indent}command = ${commands[$((cur_terminal_nums-1))]}
      ${config_entry_indent}directory = ${dirs[$((cur_terminal_nums-1))]}
      "
    done

    echo "${layout_config}"
}