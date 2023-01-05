#!/bin/bash

target_folder=$(realpath "$1")
source_folder=$(realpath "$2")

hashes () {
  hash=$(shasum -a 256 "$1" | head -c 16)
	chars=$(echo "$hash" | grep -o '.')
	path=""
	for char in $chars; do
		path="$path/$char"
		mkdir -p "$target_folder$path"
	done
	
  cp "$item" "$target_folder/$path/$hash"

  echo "$hash" > "$item"
}

find $source_folder -type f | while read file; do hashes $file; done
