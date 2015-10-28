#!/bin/sh

DATA_COUNT=`wc -l doc/dataset.txt | cut -d ' ' -f 1`
SET_SIZE=`expr $DATA_COUNT / 10`
REMAINDER=`expr $DATA_COUNT % 10`

echo "spliting dataset...(into doc/split)"
echo "  data size: $DATA_COUNT"
echo "subset size: $SET_SIZE"
echo "  remainder: $REMAINDER\n"

if [ ! -d "doc/split" ]; then
    mkdir doc/split
fi

for i in 0 1 2 3 4 5 6 7 8 9; do

    head -`echo "$i * $SET_SIZE" | bc` doc/dataset.txt > doc/split/$i.train
    head -`echo "($i + 1) * $SET_SIZE" | bc` doc/dataset.txt | tail -$SET_SIZE > doc/split/$i.test
    tail -`echo "(9 - $i) * $SET_SIZE" | bc` doc/dataset.txt >> doc/split/$i.train

    if [ $i -eq 9 ]; then
        tail -$REMAINDER doc/dataset.txt >> doc/split/$i.test
    fi

done

echo "\c" > picture/average.txt
echo "\c" > picture/all.txt
for i in `seq 101`; do

    i=`echo "$i - 1" | bc`
    PREFIX="0"
    if [ $i -eq 0 ]; then
        PREFIX="0.0"
    elif [ $i -eq 100 ]; then
        PREFIX=""
    fi
    threshold=$PREFIX`echo "$i * 0.01" | bc -l`

    sum="0"

    if [ $i -eq 0 ]; then
        echo "set the threshold value to $threshold"
        echo "performing 10-fold cross-validation..."
        echo "\t Train Set\tTest Set\tCorrect Rate"
        echo "\t+--------------------------------------------"
    fi
    for j in 0 1 2 3 4 5 6 7 8 9; do

        correct_rate=`./bin/Decision doc/split/$j.train doc/split/$j.test $threshold`
        correct_rate_=`echo $correct_rate | cut -d "%" -f 1`
        sum=`echo "$sum + $correct_rate_" | bc -l`

        if [ $i -eq 0 ]; then
            echo "\t $j.train\c"
            if [ $i -ne 10 ]; then
                echo "\t\c"
            fi
            echo "$j.test\t\t\c"
            echo $correct_rate
        fi
        echo "$threshold,$j,$correct_rate_" >> picture/all.txt

    done

    average_correct_rate=`echo "scale=6;$sum / 10" | bc -l`

    if [ $i -eq 0 ]; then
        echo "\t+--------------------------------------------"
        echo "\t average correct rate:\t\t$average_correct_rate%\n"
        echo "\nperforming 10-fold cross-validation for threshold value from 0.0 to 1.0 with increment 0.01...\n"
    fi

    echo "$threshold,$average_correct_rate" >> picture/average.txt
    if [ $i -lt 6 ]; then
        echo "$threshold,$average_correct_rate"
    elif [ $i -eq 6 ]; then
        echo "...(please wait for a few seconds)"
    elif [ $i -gt 95 ]; then
        echo "$threshold,$average_correct_rate"
    fi

    if [ $i -eq 100 ]; then
        echo "\nthe useful data has been written into file picture/average.txt and picture/all.txt"
    fi

done
