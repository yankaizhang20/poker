#!/bin/bash

source $(dirname $0)/utils/utils.bash || { exit 1; }

safe source $(dirname $0)/var/poker_var.bash

# @brief: 将指定目录下的所有指定文件按照指定格式格式化
# $1 指定目录  $2 指定文件模式  $3 格式文件位置

function Format()
{
  # 若不存在 clang_format 命令，则报错
  if ! command -v clang-format &>/dev/null; then
    echo "can not find clang_format"
    exit 1
  fi

  # 若给定的.clang_format 文件不存在，则报错
  if [[ ! -e $3 ]];then
    echo $3
    echo ".clang_format file doesn't exist"
    exit 1
  fi

  # 查找指定目录下的所有符合模式的文件
  file_list=$(find "$1" -type f -writable $2) || { echo "文件查找失败"; exit 1; }

  echo "${POKER_TASK_TAG} format task begin for pattern \"$2\""

  for file in ${file_list}; do
    # 使用相应的 .clang_format 格式化
    if ! clang-format --style=file:$3 -i ${file}; then
      echo "?? ${file} cann't be formatted"
    else
      echo "${POKER_INDENT}${POKER_TASK_COM_TAG} format ${file} successfully"
    fi
  done
}