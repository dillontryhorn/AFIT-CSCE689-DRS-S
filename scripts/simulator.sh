#!/bin/bash
#Created by Dillon Tryhorn
#For CSCE689 DRS Final Project

simulation() {
	output="$(../src/divfinder ${1} ${2} ${3} ${4} | grep Time)"
	echo "Number: ${1} $output" >> resultsT2.31-2.32v2threads.txt
}

#Mark Start
date
max=$((2**32))
counter=`expr $max - 100000`
while [ $counter -ne $max ]
do
	simulation "$counter" "0" "2" "false"
	((counter++))
done
date
#Mark End

