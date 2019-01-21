#/bin/bash

rm -f output*.txt
rm -f input.txt
make -C build/
make -C optimize_build/

echo "Generating input..."
./optimize_build/column1_gen_numbers > input.txt

echo "Debug set sort..."
/usr/bin/time -f "%e" cat input.txt | ./build/set_sort > output1.txt
echo "Release set sort..."
/usr/bin/time -f "%e" cat input.txt | ./optimize_build/set_sort > output2.txt

echo "Debug bitmap sort..."
/usr/bin/time -f "%e" cat input.txt | ./build/bitmap_sort > output3.txt
echo "Release bitmap sort..."
/usr/bin/time -f "%e" cat input.txt | ./optimize_build/bitmap_sort > output4.txt

echo "System sort..."
/usr/bin/time -f "%e" sort -n input.txt > output5.txt

echo "Low memory bitmap sort..."
/usr/bin/time -f "%e" ./optimize_build/bitmap_sort_low_memory input.txt > output6.txt

diff output1.txt output2.txt
diff output1.txt output3.txt
diff output1.txt output4.txt
diff output1.txt output5.txt
diff output1.txt output6.txt
