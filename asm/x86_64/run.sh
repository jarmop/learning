#!/bin/bash

if [ -z ${1+x} ]; then
    echo "Please give filename"
    exit 1;
fi

filename=$1

IFS='.' read -a filename_parts <<< "$filename"

file_basename="${filename_parts[0]}"

yasm -f bin $filename -o "$file_basename".bin
dd if="$file_basename".bin of="$file_basename".img
VBoxManage startvm boot-test
