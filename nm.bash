#!/bin/bash

########################################################################
#                                                                      #
# Copyright(C) 2017 - LBS - (Single person developer.)                 #
# Thu Oct 26 16:24:03 JST 2017                                         #
# Autor: Leonid Burmistrov                                             #
#                                                                      #
# File description:                                                    #
#                  This script prints list the functions in a shared   #
#                  library.                                            #
#                                                                      #
# Input paramete:                                                      #
#                  NON.                                                #
#                                                                      #
# This software is provided "as is" without any warranty.              #
#                                                                      #
########################################################################

function nm_bash {

    liblist=$1
    
    for filename in $liblist ; do
	echo " "
	echo "----> $filename"
	nm -CD --defined-only $filename | awk '{if($2=="T"){{print }}}' | grep -v _fini | grep -v _init | nl
	echo " "
    done
    
}

nm_bash "./obj/*.so"
