#!/bin/tcsh

setenv APEXHOME /home/oliver/APEX

# for Root 
setenv ROOTSYS /cern/root/v6.20.04
#setenv ROOTSYS /cern/root/v6.14.04b
setenv PATH "$PATH":$ROOTSYS/bin
setenv LD_LIBRARY_PATH "$LD_LIBRARY_PATH":$ROOTSYS/lib

# for Analayzer                                               
setenv ANALYZER /home/oliver/APEX/analyzer
setenv LD_LIBRARY_PATH "$LD_LIBRARY_PATH":$ANALYZER/lib64
setenv PATH "$PATH":$ANALYZER/bin
setenv DB_DIR /home/oliver/APEX/HallA-APEX-Online/replay/DB

# For HPSTR/LCIO
setenv LCIO_DIR $APEXHOME/HPS_code/hps-lcio/install
#setenv LCIO_INCLUDE_DIRS $LCIO_DIR/include
setenv LCIO_INCLUDE_DIR $LCIO_DIR/include
setenv IO_LCIO_LIBRARY $LCIO_DIR/lib/liblcio.so
#setenv LD_LIBRARY_PATH "LD_LIBRARY_PATH":$LCIO_DIR/lib
setenv LCIO_LIBRARIES $LCIO_DIR/lib
setenv PATH "$PATH":$LCIO_DIR/bin

echo Cern Root $ROOTSYS
echo JLab Hall A Analyzer $ANALYZER
