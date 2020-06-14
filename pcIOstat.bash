#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2019 - LBS - (Single person developer.)                 #
# Sun Apr 28 17:30:08 JST 2019                                         #
# Autor: Leonid Burmistrov                                             #
#                                                                      #
# File description:                                                    #
#                                                                      #
# Input paramete:                                                      #
#                                                                      #
# This software is provided "as is" without any warranty.              #
#                                                                      #
########################################################################

LC_TIME=en_US.UTF-8

#source /home/hvala/root/bin/thisroot.sh
#source /home/burmist/root_v6.14.00/root-6.14.00-install/bin/thisroot.sh
#source /home/usr/hvala/root_v5.34.34/root/bin/thisroot.sh
#arichmon
source /home/usr/hvala/pcstatlog/setupEnv.bash
source /home/usr/hvala/belle2/externals/v01-07-01/Linux_x86_64/opt/root/bin/thisroot.sh

function pcIOstat_bash {
    echo "$sourceHome/pcIOstat"
    for i in `seq 1 $numberOfMeasurements`;
    do
	#ariterm1
	sdbave_occupancy=`(iostat -dxy | grep sdb | awk {'print $14'})`
	sdcave_occupancy=`(iostat -dxy | grep sdc | awk {'print $14'})`
	sdb_occupancy=`(iostat -dxy $timeOfMeasurements 1 | grep sdb | awk {'print $14'})`
	sdc_occupancy=`(iostat -dxy $timeOfMeasurements 1 | grep sdc | awk {'print $14'})`
	echo "sdb_occupancy    = $sdb_occupancy"
	echo "sdc_occupancy    = $sdc_occupancy"
	echo "sdbave_occupancy = $sdbave_occupancy"
	echo "sdcave_occupancy = $sdcave_occupancy"
	$sourceHome/pcIOstat 0 $outRootFile $timeOfMeasurements 4 $sdbave_occupancy $sdcave_occupancy $sdb_occupancy $sdc_occupancy
	#$sourceHome/pcIOstat 0 $outRootFile $timeOfMeasurements 1 $sdb_occupancy
    done
}

function pcIOstat_test {
    echo "$sourceHome/pcIOstat : pcIOstat_test"
    $sourceHome/pcIOstat 0 $outRootFile $timeOfMeasurements 1 1
    $sourceHome/pcIOstat 0 $outRootFile $timeOfMeasurements 2 2 2.5
    $sourceHome/pcIOstat 0 $outRootFile $timeOfMeasurements 3 3 3.3 3.5
    $sourceHome/pcIOstat 0 $outRootFile $timeOfMeasurements 1 4.0
}

function printHelp {
    echo " --> ERROR in input arguments "
    echo " [0] -d : default"
    echo " [0] -t : test"
    echo " [0] -h : print help"
}

sourceHome="/home/usr/hvala/pcstatlog/"
#sourceHome="/home/hvala/pcstatlog/"
#sourceHome="/home/burmist/pcstatlog/"
dataFolder=$sourceHome'data/'
outRootFile=$dataFolder$HOSTNAME-'pcIOstat.root'
mkdir -p $dataFolder
timeOfMeasurements=60
numberOfMeasurements=3
#timeOfMeasurements=2
#numberOfMeasurements=10

if [ $# -eq 0 ] 
then    
    printHelp
else
    if [ "$1" = "-d" ]; then
	if [ $# -eq 1 ]
        then
	    pcIOstat_bash
        else
            printHelp
        fi
    elif [ "$1" = "-t" ]; then
        pcIOstat_test
    elif [ "$1" = "-h" ]; then
        printHelp
    else
        printHelp
    fi
fi

#espeak "I have done"
