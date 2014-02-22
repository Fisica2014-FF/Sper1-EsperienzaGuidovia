#!/bin/bash
# /a.out $(find ./DatiGrezzi -name "*" | grep -v ".\/DatiFormattati$" | tr "\n" " ");

rm analizzatore_dati.out
 g++_ottimizza_verboso -o ./analizzatore_dati.out ./src/AnalizzaDati.cpp
 ./analizzatore_dati.out 1>./Altro/Risultati/Prima_giornata__senza_carico.txt 2>/dev/null
