#!/bin/sh

if [ -z "$1" ]; then
    cat >&2 <<EOF
Usage: $0 foo
  compiles  map_rectangular.c
  into      workspace_switcher
\$CC overrides the choice of C compiler.
EOF
    exit 1
fi

set -xe

${CC:-cc} -O2 -Wall -std=gnu99 -pedantic            \
  -I/usr/include -L/usr/lib/x86_64-linux-gnu \
  -o workspace_switcher main.c map_$1.c \
 -lX11 -lXi