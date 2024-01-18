#!/bin/bash

# Skrypt w podanym katalogu (i podkatalogach) wyszukuje najnowszy plik, jednocześnie większy niż podana wielkość w bajtach.

if (( $# == 2 )); then
  katalog="$1"
  wielkosc="$2"

  if [[ -d "$katalog" ]]; then
    if (( wielkosc > 0 )); then
      najnowszy_plik=$(find $1 -type f -size +$2c | ls -c -lt | head -2 | tail -1 | awk -F' ' '{print($9)}')

      if [[ -n "$najnowszy_plik" ]]; then
        echo "Najnowszy plik większy niż $wielkosc bajtów: $najnowszy_plik"
      else
        echo "Brak plików spełniających kryteria."
      fi
    else
      echo "Błąd: Wielkość musi być większa od 0."
    fi
  else
    echo "Błąd: '$katalog' nie jest katalogiem."
  fi
else
  echo "Błąd: Nieprawidłowa liczba argumentów. Użycie: $0 <katalog> <wielkość>"
fi
