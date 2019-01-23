#/bin/bash

DEBUG=../build/
RELEASE=../optimize_build/
DATA=./test_data
INPUT=$DATA/input.txt

die() { echo "$*" 1>&2 ; exit 1; }

rm -f output*.txt
rm -f input.txt
make -C $DEBUG && make -C $RELEASE || die "Failed to build correctly!"

mkdir -p $DATA

echo "Generating input..."
$RELEASE/column1_gen_numbers > $INPUT || die "Failed to generate input!"

echo "System sort..."
/usr/bin/time -f "%e" sort -n $INPUT > $DATA/output.txt

echo "Debug set sort..."
/usr/bin/time -f "%e" cat $INPUT | $DEBUG/set_sort > $DATA/output1.txt
echo "Release set sort..."
/usr/bin/time -f "%e" cat $INPUT | $RELEASE/set_sort > $DATA/output2.txt

echo "Debug bitmap sort..."
/usr/bin/time -f "%e" cat $INPUT | $DEBUG/bitmap_sort > $DATA/output3.txt
echo "Release bitmap sort..."
/usr/bin/time -f "%e" cat $INPUT | $RELEASE/bitmap_sort > $DATA/output4.txt

echo "Low memory bitmap sort..."
/usr/bin/time -f "%e" $RELEASE/bitmap_sort_low_memory $INPUT > $DATA/output5.txt

diff $DATA/output.txt $DATA/output1.txt
diff $DATA/output.txt $DATA/output2.txt
diff $DATA/output.txt $DATA/output3.txt
diff $DATA/output.txt $DATA/output4.txt
diff $DATA/output.txt $DATA/output5.txt
