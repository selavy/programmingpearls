#/bin/bash

rm -f output*.txt
rm -f input.txt
make -C build/
make -C optimize_build/

echo "Generating input..."
./column_1/gen_numbers_with_dups.py > input.txt

# cat input.txt | ./build/column1_bitmap_sort_duplicates > output1.txt
time cat input.txt | ./optimize_build/column1_bitmap_sort_duplicates > output1.txt
time cat input.txt | sort -n > output2.txt

diff output1.txt output2.txt

