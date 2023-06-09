#!/bin/tcsh
#PBS -N APEX_Mass_Search
#PBS -m abe
#PBS -M oliver.jevons@glasgow.ac.uk
#PBS -l walltime=24:00:00
#PBS -V

#######################################################
# Script to run mass search at one mass hypothesis
# wrapper script
#######################################################


echo "MASS_H is $MASS_H"
echo "NBg = $NBg"
echo "NSig = $NSig"
echo "WF = $WF (window factor)"
echo "PO = $PO (Polynomial Order)"
echo "TPO = $TPO (Polynomial Order)"
echo "MT = $MT (Model type)"

# create temporary directory
set tempdir = "/scratch/oliver/APEX_mass_search_"$MASS_H$NBg$WF$PO
mkdir -p $tempdir
set infile = $tempdir/infile
mkdir -p $infile
set output = "$tempdir"/output
mkdir -p $output
set log = "$tempdir"/log
mkdir -p $log

echo "Making temporary working directory: $tempdir"

# input ROOT file location
#cp /w/work2/home/oliver/APEX/invmass/JWTrim50k/combmass_15MeV_blind.root $infile/$MASS_H$NBg$WF$PO.root
#cp /w/work2/home/oliver/APEX/invmass/50kReplay_LVDCgap2/combmass_15MeV_blind.root $infile/$MASS_H$NBg$WF$PO.root
cp /w/work2/home/oliver/APEX/invmass/TrimAsJW/combmass_15MeV_NoZero.root $infile/$MASS_H$NBg$WF$PO.root

# set environment variables

setenv ROOTSYS /cern/root/v6.20.04
setenv PATH "$PATH"/$ROOTSYS/bin
setenv LD_LIBRARY_PATH $ROOTSYS/lib

setenv LCIO_DIR /home/oliver/APEX/HPS_code/hps-lcio/install
setenv LCIO_INCLUDE_DIR $LCIO_DIR/include
setenv IO_LCIO_LIBRARY $LCIO_DIR/lib/liblcio.so
setenv LD_LIBRARY_PATH "$LD_LIBRARY_PATH":$LCIO_DIR/lib
setenv PATH  "$PATH":$LCIO_DIR/bin

alias python "/usr/local/anaconda3/bin/python"

source /home/oliver/APEX/HPS_code/hpstr/install/bin/setup.csh

echo "PATH = $PATH"
echo "LD_LIBRARY_PATH = $LD_LIBRARY_PATH"

echo "attempt hpstr script"
echo "Input file: = "$infile/$MASS_H$NBg$WF$PO.root

hpstr "$HPSTR_BASE"/processors/config/bhToys_cfg.py -i "$infile/"$MASS_H$NBg$WF$PO.root -s masshist -d "$output" -m "$MASS_H" -N "$NBg" -w "$WF" -p "$PO" -P "$TPO" -M "$MT" -r 1.0 --sig "$NSig" >& "$log"/bhToys_m"$MASS_H"w"$WF"p"$PO"r100s0.log # no signal (more recent)
#hpstr "$HPSTR_BASE"/processors/config/bhToys_cfg.py -i "$infile/"$MASS_H$NBg$WF$PO.root -s masshist -d "$output" -m "$MASS_H" -N "$NBg" -w "$WF" -p "$PO" -P "$TPO" -M "$MT" -r 1.0 --sig "$NSig" # no signal (more recent)

# move output file to work disks

echo "output file = " $output/bhToys_m"$MASS_H"w"$WF"p"$PO"r100s0.root

# add notes of bin sizes used to output; simple adjustment required to plotting scripts also

#mv $output/bhToys_m"$MASS_H"w"$WF"p"$PO"r100s"$NSig".root /w/work2/home/oliver/PeakSearch/DoubleMassRes/bhToys_0.15Bin_M"$MT"m"$MASS_H"w"$WF"p"$PO"r100s"$NSig"N"$NBg"P"$TPO".root # current

### MOVE OUTPUT TO WORKDISK AND DELETE OFF /SCRATCH/
mv $output/bhToys_m* /w/work2/home/oliver/APEX/peaksearch/NoZero_WFConst9/bhToys_0.15Bin_M"$MT"m"$MASS_H"w"$WF"p"$PO"r100s"$NSig"N"$NBg"P"$TPO".root # current
mv $log/bhToys_m"$MASS_H"w"$WF"p"$PO"r100s0.log /w/work2/home/oliver/APEX/peaksearch/logs/bhToys_0.15Bin_M"$MT"m"$MASS_H"w"$WF"p"$PO"r100s0.log

rm $infile/$MASS_H$NBg$WF$PO.root
rm $log/bhToys_m"$MASS_H"w"$WF"p"$PO"r100s0.log
rm $output/bhToys_m"$MASS_H"w"$WF"p"$PO"r100s"$NSig".root
rm -r $tempdir
