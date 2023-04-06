#!/bin/bash

if [ "$#" -gt 3 ]; then
    echo "Error : more then 3 Argument"
    exit
fi

Dir=.
AbDir=.
ListNumber=1
ListMax=0
RootFileName=SSBTree
ListName=SSBTree

if [ "$#" -ge 1 ]; then
    if [ ! "$( echo $1 | sed -e 's/[0-9]*//g' )" ]; then
	ListNumber=$1
    elif [ ! "$( echo $1 | sed -e 's/[0-9]*//g' | sed -e 's/[a-zA-Z_-]*//g' )" ]; then
	ListName=$1
    else
	Dir=$1
    fi
fi


if [ "$#" -ge 2 ]; then
    if [ ! "$( echo $2 | sed -e 's/[0-9]*//' )" ]; then
	ListNumber=$2
    elif [ ! "$( echo $2 | sed -e 's/[0-9]*//g' | sed -e 's/[a-zA-Z_-]*//g' )" ]; then
	ListName=$2
    else
	Dir=$2
    fi
fi

if [ "$#" -eq 3 ]; then
    if [ ! "$( echo $3 | sed -e 's/[0-9]*//' )" ]; then
	ListNumber=$3
    elif [ ! "$( echo $3 | sed -e 's/[0-9]*//g' | sed -e 's/[a-zA-Z_]*//g' )" ]; then
	ListName=$3
    else
	Dir=$3
    fi
fi

if [ $Dir = "." ]; then
    echo "Root File Path      : Currnet Directory"
else
    echo -e "Root File Path      : $Dir"
fi

ListMax=$( ls $Dir/${RootFileName}_*.root 2>/dev/null | wc -l )
if [ "$ListMax" -eq 0 ]; then
    echo "Error : No directory or No root file"
    exit
fi
echo -e "Number of Root File : $ListMax"
echo -e "List Name           : $ListName"
echo -e "Number of List      : $ListNumber"

if [ "$( echo -e $Dir | grep ^/.* )" ]; then
    AbDir=$Dir
else
    cd $(pwd)"/"$Dir
    AbDir=$(pwd)
    cd - > /dev/null
fi

ls $AbDir/${RootFileName}_*.root | grep SSBTree_[0-9]_.*\.root > ${ListName}.list
ls $AbDir/${RootFileName}_*.root | grep SSBTree_[0-9][0-9]_.*\.root >> ${ListName}.list
ls $AbDir/${RootFileName}_*.root | grep SSBTree_[0-9][0-9][0-9]_.*\.root >> ${ListName}.list
ls $AbDir/${RootFileName}_*.root | grep SSBTree_[0-9][0-9][0-9][0-9]_.*\.root >> ${ListName}.list

if [ "$ListNumber" -gt 1 ]; then
    index=1
    ListDiv=$((ListMax/ListNumber))
    ListEnd=$((ListDiv+1))
    while [ "$index" -le "$ListNumber" ]; do
	sed -n "$((ListEnd-ListDiv))","$ListEnd"p ${ListName}.list > ${ListName}_${index}.list
	index=$((index+1))
	ListEnd=$((ListEnd+ListDiv+1))
    done
fi

exit
