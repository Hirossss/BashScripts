#!/bin/bash

if (( $# == 2 )); then
    podstawa="$1"
    potega="$2"

    # Rozwiązanie operatorem
    wynik_operatorem=$((podstawa ** potega))
    echo "Rozwiązanie operatorem: $podstawa^$potega = $wynik_operatorem"

    # Rozwiązanie pętlą
    wynik_petla=1
    for (( i=1; i<=potega; i++ )); do
        wynik_petla=$((wynik_petla * podstawa))
    done
    echo "Rozwiązanie pętlą: $podstawa^$potega = $wynik_petla"
else
    echo "Błąd: Nieprawidłowa liczba argumentów. Użycie: $0 <podstawa> <potega>"
fi
