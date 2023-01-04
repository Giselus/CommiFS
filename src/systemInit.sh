#!/bin/bash

script_name=$0
target_folder=$1
source_folder=$2
script_path=$(pwd)/${script_name:2}

# echo $script_path

cd "$source_folder"

for item in *; do

  if [[ -d "$item" ]]; then
    bash "$script_path" "$target_folder" "$item"
  else
  
    hash=$(shasum -a 256 "$item" | cut -d ' ' -f 1)
    hash=$(echo "$hash" | tr '[:lower:]' '[:upper:]')
    hash=${hash:0:16}

	chars=$(echo "$hash" | grep -o '.')

	path=""
	for char in $chars; do
		path="$path/$char"
		mkdir -p "$target_folder$path"
	done
	
    cp "$item" "$target_folder/$path/$hash"

    echo "$hash" > "$item"
  fi
done