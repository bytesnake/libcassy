#!/bin/sh

echo "Forcing source files to unix format..."
find . -type f -name '*.[ch]' -exec dos2unix -q {} \+

echo "Removing trailing whitespace from source files..."
find . -type f -name '*.[ch]' -exec sed -i 's/[[:space:]]\+$//' {} \+

echo "Adding trailing newline to source files..."
find . -type f -name '*.[ch]' -exec sed -i -e '$a\' {} \+

echo "Done!"