############################################################### -*- Makefile -*-
#
# Filename:  Makefile
#
# Summary:   
# 
# Notes:
#   This file is only for use with GNU make version 3.78.1 or greater
#
################################################################################
.PHONY : debug release

DBFLAGS=

debug :
	${MAKE} default DBFLAGS="-O0 -g -DBART -DDEBUGGING"

release :
	${MAKE} default DBFLAGS="-O3 -DBART"
time :
	${MAKE} default DBFLAGS="-O3 -DTIMER -DBART"

C++FLAGS = -DSSV_LOCALISATION \
	-DRANGE_QUERIES \
	-fPIC

LDFLAGS += -L/apps/gcc/v4.5.0/lib64

LIBS = -lpthread \
	-lstdc++

C++ = g++ 

include libpdesmas.mk
