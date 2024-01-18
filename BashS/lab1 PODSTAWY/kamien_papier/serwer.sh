#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Użycie: $0 <liczba_gier>"
  exit 1
fi

LICZBA_GIER=$1
SUMA_WYNIKOW=0
REMIS=0

# Funkcja do uruchamiania gracza
uruchom_gracza() {
  ./gracz"$1".sh &
  GRACZ_PID=$!
  GRACZE_PIDS+=("$GRACZ_PID")
}


# Uruchomienie graczy
uruchom_gracza 1
uruchom_gracza 2

for ((i=1; i<=$LICZBA_GIER; i++)); do
  echo "Rozpoczynam grę $i"
  echo "start" > komenda.txt

  while [ ! -f los1.txt ] || [ ! -f los2.txt ]; do
    sleep 0.1
  done

  sleep 0.1
  rm -f komenda.txt

  LOS1=$(cat los1.txt)
  LOS2=$(cat los2.txt)

  # Rozstrzygnięcie wyniku
  if [ "$LOS1" == "$LOS2" ]; then
    WYNIK="Remis"
    ((REMIS++))
    echo "$WYNIK"
  elif [ "$LOS1" == "papier" ] && [ "$LOS2" == "kamien" ]; then
    WYNIK="Gracz 1 wygrywa"
    echo "$WYNIK"
    ((SUMA_WYNIKOW++))
  elif [ "$LOS1" == "kamien" ] && [ "$LOS2" == "nozyce" ]; then
    WYNIK="Gracz 1 wygrywa"
    echo "$WYNIK"
    ((SUMA_WYNIKOW++))
  elif [ "$LOS1" == "nozyce" ] && [ "$LOS2" == "papier" ]; then
    WYNIK="Gracz 1 wygrywa"
    echo "$WYNIK"
    ((SUMA_WYNIKOW++))
  else
    WYNIK="Gracz 2 wygrywa"
    echo "$WYNIK"
  fi

  echo "$WYNIK" >> gra.log
  rm -f los1.txt los2.txt
done

echo "Sumaryczny wynik: Gracz 1: $SUMA_WYNIKOW, Gracz 2: $((LICZBA_GIER - SUMA_WYNIKOW - REMIS))"
echo ""
echo "stop" > komenda.txt
sleep 1
rm -f komenda.txt
rm -f gra.log
echo "Zakończono działanie."

