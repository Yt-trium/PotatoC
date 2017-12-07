#!/bin/bash

make

echo "Will run potatoc on all test files.";
echo "Saving log in test.log";
FAIL=0;
TOTAL=0;
cp "/dev/null" "test.log";

set -o pipefail # To get exit code of piped command

echo
for f in tests/*.txt ; 
do 
    echo "Running $f...";
    echo -e "$f\n" >> "test.log";
    ./bin/potatoc < $f >> "test.log" 2>&1;
    CODE=${PIPESTATUS[0]};
    echo -e "\n\n" >> "test.log";
    if [ "${CODE}" -ne "0" ]
    then
        echo  "Failed.";
        FAIL=$((FAIL+1));
    else
        echo  "OK.";
    fi
    TOTAL=$((TOTAL+1))
done

echo
echo "$((TOTAL-FAIL)) on $TOTAL has succeeded.";
