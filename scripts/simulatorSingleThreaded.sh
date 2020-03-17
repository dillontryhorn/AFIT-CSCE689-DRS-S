#!/bin/bash
#Created by Dillon Tryhorn
#For CSCE689 DRS Final Project

simulation() {
	output="$(../src/divfinder ${1} ${2} ${3} ${4} | grep Time)"
	echo "Number: ${1} $output" >> resultsSP2.31-2.32v2.txt
}

#Mark Start
date
max=$((2**32))
counter=`expr $max - 100000`
while [ $counter -ne $max ]
do
	simulation "$counter" "0" "4" "false"
	((counter++))
done
date
#Mark End

