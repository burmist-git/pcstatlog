########################################################################
#                                                                      #
# Copyright(C) 2019 - LBS - (Single person developer.)                 #
# Tue Apr 16 13:42:43 JST 2019                                         #
# Autor: Leonid Burmistrov                                             #
#                                                                      #
# Script description:                                                  #
#                                                                      #
# Input paramete:                                                      #
#                                                                      #
# This software is provided "as is" without any warranty.              #
########################################################################

ROOTCFLAGS  = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS    = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS   = $(shell $(ROOTSYS)/bin/root-config --glibs)
ROOTLDFLAGS = $(shell $(ROOTSYS)/bin/root-config --ldflags)

# add xml files
#ROOTLIBS += -lMLP -lXMLIO

CXX  = g++
CXX += -I./

CXXFLAGS  = -g -Wall -fPIC -Wno-deprecated
CXXFLAGS += $(ROOTCFLAGS)
CXXFLAGS += $(ROOTLIBS)
CXXFLAGS += $(ROOTGLIBS)
CXXFLAGS += $(ROOTLDFLAGS)
CXXFLAGSDICT = -g -Wall -fPIC -Wno-deprecated

OUTLIB = /home/usr/hvala/pcstatlog/obj/

.PHONY: all makedir runanapcstat clean

#----------------------------------------------------#

all: makedir pcStatLog pcIOstat runanapcstat 

makedir:
	mkdir -p $(OUTLIB);

pcStatLog: pcStatLog.cc
	$(CXX) -DPCSTATLOG_MAIN -o $@ $< $(CXXFLAGS)

pcIOstat: pcIOstat.cc
	$(CXX) -DPCIOSTAT_MAIN -o $@ $< $(CXXFLAGS)

obj/pcIOstat.o: pcIOstat.cc libpcIOstat.h
	$(CXX) $(CXXFLAGS) -c -I. -o $@ $<

obj/libpcIOstat.so: obj/pcIOstat.o
	$(CXX) -shared -o $@ $^ $(ROOTLIBS) $(ROOTGLIBS)

runanapcstat: obj/libpcIOstat.so
	make -f Makefileanapcstat

clean:
	rm -f *~
	rm -f .*~
	rm -f pcStatLog
	rm -f pcIOstat
	make -f Makefileanapcstat clean
