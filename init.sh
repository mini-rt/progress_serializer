#!/bin/sh

set -e

mkdir -p lib
echo "*" > lib/.gitignore
git -C lib clone https://github.com/mini-rt/base.git
