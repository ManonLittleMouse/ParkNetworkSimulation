cd build
cd src

for i in 100 300 500 
do
    for f in 100 10
        do
            for repeat in {0. .20}
                do
                    echo $i $f 
                    ./main $i $f 
                    sleep 2
                done
        done
done

