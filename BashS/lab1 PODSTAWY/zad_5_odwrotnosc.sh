#!/bin/bash

params=($*)
n=$#

for ((i=$n-1;i>=0;i--))
do
        echo -n " ${params[i]}"
done

echo ""
