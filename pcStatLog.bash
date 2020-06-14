#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2019 - LBS - (Single person developer.)       1          #
# Tue Apr 16 10:58:48 JST 2019                                         #
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

function pcStatLog_bash {
    #df -h /
    #Filesystem      Size  Used Avail Use% Mounted on
    #/dev/nvme0n1p2  239G   77G  150G  35% /
    #df -h $HOME
    #Filesystem      Size  Used Avail Use% Mounted on
    #/dev/nvme0n1p2  239G   77G  150G  35% /
    #rootUsed=`(df -h / | grep / | awk {'print $3'} | sed 's/\T/000/' | tr -cd [:digit:])`
    #rootAvail=`(df -h / | grep / | awk {'print $4'} | sed 's/\T/000/' | tr -cd [:digit:])`
    #homeUsed=`(df -h $HOME | grep / | awk {'print $3'} | sed 's/\T/000/' | tr -cd [:digit:])`
    #homeAvail=`(df -h $HOME | grep / | awk {'print $4'} | sed 's/\T/000/' | tr -cd [:digit:])`
    #rootUsed=`(df -h / | grep / | awk {'print $3'} | sed 's/\,/./' | sed 's/\G//')`
    #rootAvail=`(df -h / | grep / | awk {'print $4'} | sed 's/\,/./' | sed 's/\G//')`
    #homeUsed=`(df -h $HOME | grep / | awk {'print $3'} | sed 's/\,/./' | sed 's/\G//')`
    #homeAvail=`(df -h $HOME | grep / | awk {'print $4'} | sed 's/\,/./' | sed 's/\G//' )`
    #rootUsed=`(df -h / | grep / | awk {'print $3'} | tr -cd [:digit:])`
    #rootAvail=`(df -h / | grep / | awk {'print $4'} | tr -cd [:digit:])`
    #homeUsed=`(df -h $HOME | grep / | awk {'print $3'} | tr -cd [:digit:])`
    #homeAvail=`(df -h $HOME | grep / | awk {'print $4'} | tr -cd [:digit:])`
    rootUsed=`(df / | grep / | awk {'print $2'} | tr -cd [:digit:])`
    rootAvail=`(df / | grep / | awk {'print $3'} | tr -cd [:digit:])`
    homeUsed=`(df $HOME | grep / | awk {'print $3'} | tr -cd [:digit:])`
    homeAvail=`(df $HOME | grep / | awk {'print $4'} | tr -cd [:digit:])`
    echo "rootUsed  "$rootUsed
    echo "rootAvail "$rootAvail
    echo "homeUsed  "$homeUsed
    echo "homeAvail "$homeAvail
    #avg-cpu:  %user   %nice %system %iowait  %steal   %idle
    #           5,15    0,00    1,35    0,03    0,00   93,46
    #avgcpuuser=`(iostat -c | grep -v % | grep , | awk {'print $1'} | sed 's/\,/./')`
    avgcpuuser=`(iostat -c | grep -v % | grep . | grep -v Linux | awk {'print $1'})`
    #avgcpuusersystem=`(iostat -c | grep -v % | grep , | awk {'print $3'} | sed 's/\,/./')`
    avgcpuusersystem=`(iostat -c | grep -v % | grep . | grep -v Linux | awk {'print $3'})`
    #avgcpuuseridle=`(iostat -c | grep -v % | grep , | awk {'print $6'} | sed 's/\,/./')`
    avgcpuuseridle=`(iostat -c | grep -v % | grep . | grep -v Linux | awk {'print $6'})`
    echo "avgcpuuser       $avgcpuuser"
    echo "avgcpuusersystem $avgcpuusersystem"
    echo "avgcpuuseridle   $avgcpuuseridle"
    #free -m
    #              total        used        free      shared  buff/cache   available
    #Mem:          15613        4605        7835         819        3173        9823
    #Swap:         30516           0       30516
    #ariterm1
    #memtotal=`(free -m | grep Mem: | awk {'print $2'})`
    #memused=`(free -m | grep Mem: | awk {'print $3'})`
    #memfree=`(free -m | grep Mem: | awk {'print $4'})`
    #arichsc
    #memtotal=`(free -m | grep Mem: | awk {'print $2'})`
    #memused=`(free -m | grep Mem: | awk {'print $3'})`
    #memfree=`(free -m | grep Mem: | awk {'print $4'})`
    #arichmon
    memtotal=`(free -m | grep Mem: | awk {'print $2'})`
    memused=`(free -m | grep cache: | awk {'print $3'})`
    memfree=`(free -m | grep cache: | awk {'print $4'})`
    echo "memtotal $memtotal"
    echo "memused  $memused"
    echo "memfree  $memfree"
    #
    echo "$sourceHome/pcStatLog"
    $sourceHome/pcStatLog 0 $outRootFile $rootUsed $rootAvail $homeUsed $homeAvail $avgcpuuser $avgcpuusersystem $avgcpuuseridle $memtotal $memused $memfree
} 

function printHelp {

    echo " --> ERROR in input arguments "
    echo " [0] -d  : default"
    echo " [0] -p2 : other"
    echo " [0] -h  : print help"

}

sourceHome="/home/usr/hvala/pcstatlog/"
#sourceHome="/home/hvala/pcstatlog/"
#sourceHome="/home/burmist/pcstatlog/"
dataFolder=$sourceHome'data/'
outRootFile=$dataFolder$HOSTNAME-'pcstatlog.root'
mkdir -p $dataFolder

if [ $# -eq 0 ] 
then    
    printHelp
else
    if [ "$1" = "-d" ]; then
	if [ $# -eq 1 ]
        then   
	    #echo "outRootFile = $outRootFile"
	    pcStatLog_bash
        else
            printHelp
        fi
        
    elif [ "$1" = "-p2" ]; then
            printHelp
    elif [ "$1" = "-h" ]; then
        printHelp
    else
        printHelp
    fi
fi

#espeak "I have done"
