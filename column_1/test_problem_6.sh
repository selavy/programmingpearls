#/bin/bash

DEBUG=../build/
RELEASE=../optimize_build/
DATA=./test_data_problem_6/
INPUT=$DATA/input.txt

die() { echo "$*" 1>&2 ; exit 1; }

mkdir -p $DATA

rm -f $DATA/output*.txt
rm -f $DATA/input.txt
make -C $DEBUG
make -C $RELEASE

echo "Generating input..."
./gen_numbers_with_dups.py > $INPUT || die "Failed to generate input!"

echo "System sort:"
time cat $INPUT | sort -n > $DATA/output.txt
echo "Debug:"
time cat $INPUT | $DEBUG/column1_bitmap_sort_duplicates > $DATA/output1.txt
echo "Release:"
time cat $INPUT | $RELEASE/column1_bitmap_sort_duplicates > $DATA/output2.txt

diff $DATA/output.txt $DATA/output1.txt
diff $DATA/output.txt $DATA/output2.txt

