make
# clear
> output.txt

echo Enter lower limit of integral:
read a
echo Enter upper limit of integral:
read b

for i in {1000..10000..500}
do
    echo ::::::::::::::::::::::::::::::: >> output.txt
    echo Intervals: $i >> output.txt
    for mode in {1..3..1}
    do
        ./integral $a $b $i $mode >> output.txt
    done
done

echo Done!