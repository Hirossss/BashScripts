#!/bin/bash

# Instrukcja warunkowa sprawdzajaca, czy user podal wszystkie argumenty
if [ "$#" -ne 4 ]
then
	echo "Użycie: ./skrypt04.sh <dokument.pdf> <podpis.png> <x> <y>"
	exit 1
fi

# Przypasanie argumentow do zmiennych
PDF_FILE="$1"
SIGNATURE_FILE="$2"
X="$3"
Y="$4"

# Instrukcja warunkowa sprawdzajaca, czy plik PDF istnieje
if [ ! -f "$PDF_FILE" ]
then
	echo "Plik PDF '$PDF_FILE' nie istnieje."
	exit 1
fi

# If sprawdzajacy, czy plik PNG istnieje
if [ ! -f "$SIGNATURE_FILE" ]; then
	echo "Plik PNG '$SIGNATURE_FILE' nie istnieje."
	exit 1
fi

# ImageMagick naklada napis (obrazek PNG) na plik w formacie PDF
output_file=$PDF_FILE
convert "$PDF_FILE" "$SIGNATURE_FILE" -geometry +${X}+${Y} -composite "$output_file"

# Wyswietlenie informacji o pomyslnym podpisaniu podanego PDFa
echo "Podpis zostal nałozony na plik PDF: $output_file"

