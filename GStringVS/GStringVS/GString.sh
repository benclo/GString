#!/bin/bash
#SBATCH -p main      # Queue name
#SBATCH --mem=188000

# Compiler and flags
GCC="g++ -O3"

# List all source files
SRC_FILES="GStringVS.cpp Trie.cpp utils.cpp Graph.cpp"

# List all object files
OBJ_FILES="GStringVS.o Trie.o utils.o Graph.o"

# Compile individual object files
for src in $SRC_FILES; do
    obj="${src%.cpp}.o"
    $GCC -c $src -o $obj
done

$GCC -o GString $OBJ_FILES

./GString
./GString
./GString
./GString
./GString