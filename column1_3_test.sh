#/bin/bash

time cat ./column_1/input.txt | ./build/column1_3 > output.txt
time cat ./column_1/input.txt | ./optimize_build/column1_3 > output.txt
time sort -n ./column_1/input.txt > output2.txt
