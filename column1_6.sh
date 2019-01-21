#/bin/bash

rm -f output*.txt
# rm -f input.txt
make -C build/
make -C optimize_build/

# echo "Generating input..."
# ./column_1/gen_numbers_with_dups.py > input.txt

cat input.txt | ./build/column1_bitmap_sort_duplicates > output1.txt
# cat input.txt | ./optimize_build/column1_bitmap_sort_duplicates > output1.txt
cat input.txt | sort -n > output2.txt

head -n 20 output1.txt > output1_short.txt
head -n 20 output2.txt > output2_short.txt
diff output1_short.txt output2_short.txt

