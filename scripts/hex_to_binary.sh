#!/bin/sh
if [ "$#" != "2" ]; then
  echo "Args: input file, output file"
  exit 1
fi
xxd -r -p test.hex test.bin


