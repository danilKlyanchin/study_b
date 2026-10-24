#/!/bin/bash

g++ -std=c++20 \
  -Wall -Wextra -Wpedantic -Wconversion -Wshadow \
  -pthread \
  -I"$(brew --prefix fmt)/include" \
  -L"$(brew --prefix fmt)/lib" \
  $1 \
  -o exec -lfmt
