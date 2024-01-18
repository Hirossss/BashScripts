#!/bin/bash

if (( $# == 2 )); then
    min="$1"
    max="$2"

    if (( min > max )); then
        echo "Błąd: min musi być mniejsze niż max."
        exit 1
    fi

    # Generate a random number between min and max
    answer=$((RANDOM % (max - min + 1) + min))

    attempts=0
    max_attempts=10  # Ogranicz liczba zgadywań

    # Start the game
    while true; do
        # Ask the user for their guess
        read -p "Guess a number between $min and $max: " guess

        if (( guess < min || guess > max )); then
            echo "Your guess is out of the specified range. Try again."
            continue
        fi

        ((attempts++))

        # Check if the guess is correct
        if (( guess == answer )); then
            echo "Congratulations, you guessed the correct number in $attempts attempts!"
            break
        elif (( guess < answer )); then
            echo "Too low, try again."
        else
            echo "Too high, try again."
        fi

        # Check if the maximum number of attempts has been reached
        if (( attempts >= max_attempts )); then
            echo "Sorry, you've reached the maximum number of attempts. The correct number was $answer."
            break
        fi
    done
else
    echo "Błąd: Nieprawidłowa liczba argumentów. Użycie: $0 <min> <max>"
fi
