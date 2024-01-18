#!/bin/bash

while true; do
  if [ -f komenda.txt ] && [ "$(cat komenda.txt 2>/dev/null)" == "start" ] && [ ! -f los1.txt ]; then
    LOS=$((RANDOM % 3))

    case $LOS in
      0) WYNIK="papier" ;;
      1) WYNIK="nozyce" ;;
      2) WYNIK="kamien" ;;
    esac

    echo "$WYNIK" > los1.txt
  elif [ -f komenda.txt ] && [ "$(cat komenda.txt 2>/dev/null)" == "stop" ]; then
    break
  fi
done
