#!/bin/bash

if [ $# -ne 2 ]; then
        echo "Uzycie: $0 <x> <y>"
        exit 1
fi

if ! [[ $1 =~ ^[0-9]+$ ]] || ! [[ $2 =~ ^[0-9]+$ ]]; then
    echo "Oba argumenty musz ^e by ^g liczbami ca ^bkowitymi."
    exit 1
fi

x_range=$1
y_range=$2
no_questions=0
no_of_successes=0

while true; do
        ((no_questions++))
        x_random=$((RANDOM % x_range + 1))
        y_random=$((RANDOM % y_range + 1))
        multiplication=$((x_random * y_random))
        echo "Ile to $x_random * $y_random ?"

        read -p "Podaj odpowiedz: " answer

        if [ $answer -le 0 ]; then
                bad_ans=$((no_questions - no_of_successes-1))
                echo "Proby: $((no_questions-1)), dobre odp: $no_of_successes, zle odp: $bad_ans"
                exit 0
        elif [ $answer -eq $multiplication ]; then
                ((no_of_successes++))
        fi
done
