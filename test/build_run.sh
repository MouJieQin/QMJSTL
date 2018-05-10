#!/bin/bash

function rm_if_exit()
{
    if [ -e $1 ]
    then
        rm $1
    fi
}

if [ $# -eq 1 ]
then 
    filename=`echo $1 | cut -d '.' -f 1`
    execfile='./'$filename
    rm_if_exit $execfile
    clang++ -std=c++14 $1 -lgtest -lgtest_main -lpthread -o $filename
    if [ -e $execfile ]
    then
        $execfile
    fi
elif [ $# -eq 0 ]
then
    rm_if_exit "./allcppfile.tmp"
    rm_if_exit "./build_fail.tmp"
    ls | grep cpp | grep test > allcppfile.tmp
    cat allcppfile.tmp | while read line
    do
        echo
        echo build and run $line ...
        echo
        filename=`echo $line | cut -d '.' -f 1`
        execfile='./'$filename
        rm_if_exit $execfile
        clang++ -std=c++14 $line -lgtest -lgtest_main -lpthread -o $filename
        if [ -e $execfile ]
        then 
            $execfile
        else
            echo $line >> build_fail.tmp
        fi
    done
    rm allcppfile.tmp
    if [ -e "./build_fail.tmp" ]
    then
        echo "--------build-----error--------"
        cat build_fail.tmp
        echo "--------build-----error--------"
        rm build_fail.tmp
    fi
else
    echo 'num of parameters error !'
fi

