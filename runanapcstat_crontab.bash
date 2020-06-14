#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2017 - LBS - (Single person developer.)                 #
# Wed Nov 21 20:39:30 JST 2018                                         #
# Autor: Leonid Burmistrov                                             #
#                                                                      #
# File description:                                                    #
#                  Program used to convert FEB temperature data in to  #
#                  2D TH2Poly histograms.                              #
#                                                                      #
# Input paramete:                                                      #
#                   NON.                                               #
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

function runanapcstat_crontab_bash {

    $sourceHome/runanapcstat $1 $2 $3 $HOSTNAME $HOME $4 $5 $6 $7 
    
}

function printHelp {
    echo " --> ERROR in input arguments "
    echo " [0] -d   : default"
    echo " [0] -ar1 : same as default + copy to ariterm1 to root_pdf_summary dir"
    echo " [0] -h   : print help"
}

sourceHome="/home/usr/hvala/pcstatlog/"
#sourceHome="/home/hvala/pcstatlog/"
#sourceHome="/home/burmist/pcstatlog/"
#sourceHomeTemp="/home/hvala/temperaturefebmergerslowcontrol/"
#sourceHomeTemp="/home/burmist/temperaturefebmergerslowcontrol/"
dataFolder=$sourceHome'data/'
inRootFile=$dataFolder$HOSTNAME-'pcstatlog.root'
#outHistRootFile=$inRootFile'_hist.root'
#outHistRootFile='/home/hvala/nishida6/data/root_pdf_summary/'$HOSTNAME-'pcstatlog_hist.root'
ariterm1_root_pdf_summary='/home/hvala/nishida6/data/root_pdf_summary/'
#outHistRootFile=$ariterm1_root_pdf_summary$HOSTNAME-'pcstatlog_hist.root'
#outHistPdfFile=$ariterm1_root_pdf_summary$HOSTNAME-'pcstatlog_hist.root.pdf'
outHistRootFile=$sourceHome'data/'$HOSTNAME-'pcstatlog_hist.root'
outHistPdfFile=$sourceHome'data/'$HOSTNAME-'pcstatlog_hist.root.pdf'
ntpstatsFilePath='/var/log/ntpstats/';
ntpstatsFileName='loopstats';
#
inRootFile_pcIO=$dataFolder$HOSTNAME'-pcIOstat.root'
inDatFile_pcIO=$sourceHome'vectorOfinDataValuesNames.dat'
#
mkdir -p $dataFolder

if [ $# -eq 0 ] 
then
    printHelp    
else
    if [ "$1" = "-d" ]; then
	if [ $# -eq 1 ]
        then
	    keyID=1
	    runanapcstat_crontab_bash $keyID $inRootFile $outHistRootFile $ntpstatsFilePath $ntpstatsFileName $inRootFile_pcIO $inDatFile_pcIO
        else
            printHelp
        fi        
    elif [ "$1" = "-ar1" ]; then
	    keyID=1
	    runanapcstat_crontab_bash $keyID $inRootFile $outHistRootFile $ntpstatsFilePath $ntpstatsFileName $inRootFile_pcIO $inDatFile_pcIO
            /home/usr/hvala/sshpass/sshpass-1.06-install/bin/sshpass -f "$sourceHome/mpfile.ariterm1" scp -r $outHistRootFile hvala@ariterm1:$ariterm1_root_pdf_summary/.
            /home/usr/hvala/sshpass/sshpass-1.06-install/bin/sshpass -f "$sourceHome/mpfile.ariterm1" scp -r $outHistPdfFile hvala@ariterm1:$ariterm1_root_pdf_summary/.
            #sshpass -f "$sourceHome/mpfile.ariterm1" scp -r $outHistRootFile hvala@ariterm1:$ariterm1_root_pdf_summary/.
            #sshpass -f "$sourceHome/mpfile.ariterm1" scp -r $outHistPdfFile hvala@ariterm1:$ariterm1_root_pdf_summary/.
    elif [ "$1" = "-h" ]; then        
        printHelp        
    else
        printHelp
    fi
fi

#espeak "I have done"
