#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Użycie: $0 <n>"
  exit 1
fi

N=$1

if ! [[ "$N" =~ ^[0-9]+$ ]]; then
  echo "Podany argument nie jest liczbą całkowitą dodatnią."
  exit 1
fi

a=0
b=1

echo "Pierwsze $N elementów ciągu Fibonacciego:"

for ((i=1; i<=$N; i++)); do
  echo -n "$a "

  temp=$a
  a=$b
  b=$((temp + a))
done

echo ""
