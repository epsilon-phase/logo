#!/usr/bin/bash
FILES="`find lib tests \( -not -name catch2.hpp \) -a -type f`"
LINES=`wc -l $FILES | tail -n 1 | cut -f 3 -d\  `
SIZE="`du -ch $FILES | tail -n 1`"
echo Project has $LINES lines of code
echo Code that is not imported measures at $SIZE


