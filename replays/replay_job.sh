#!/bin/tcsh
#PBS -N APEX_replay_50k
#PBS -m abe
#PBS -M oliver.jevons@glasgow.ac.uk
#PBS -l walltime=24:00:00
#PBS -l nodes=1
#PBS -V

#######################################################
# Script to run APEX Mass check
# wrapper script
#######################################################

# Testing single run
#setenv RUN_NUMBER 5006

echo "RUN_NUMBER is $RUN_NUMBER"

# create temporary directory
set tempdir = "/scratch/oliver/$RUN_NUMBER"
mkdir -p $tempdir
echo "Making temporary working directory: $tempdir"

# set environment variables
#setenv ROOTSYS "/cern/root/v6.20.04"
#setenv ANALYZER "/home/oliver/APEX/analyzer"
#setenv PATH "${ROOTSYS}/bin:${ANALYZER}:${PATH}"
#setenv LD_LIBRARY_PATH "${PATH}:${ROOTSYS}/lib:${ANALYZER}"

source ~/.apex_setup.csh

cd $tempdir

# copy root file to tempdir
mkdir -p log

cp /w/work5/jlab/halla/APEX/coda/apex_$RUN_NUMBER.dat* ../

echo "For run number $RUN_NUMBER"
echo "Files copied; running job"

#nohup analyzer -b -q -l "/home/oliver/APEX/APEX_Code/replays/replay_apex_trim.C\("$RUN_NUMBER",-1\)" >& $tempdir/log/$RUN_NUMBER.log
cd /home/oliver/APEX/APEX_Code/replays
analyzer -b -q  replay_apex_trim.C\($RUN_NUMBER,50000\) >& $tempdir/log/$RUN_NUMBER.log
#analyzer -b -q  replay_apex_trim.C\($RUN_NUMBER,50000\)
#analyzer -b -q  replay_apex_trim.C\($RUN_NUMBER,-1\) >& $tempdir/log/$RUN_NUMBER.log
echo "executed APEX replay for $RUN_NUMBER"

#mv root-output/apex_Trim_$RUN_NUMBER.root /w/work2/home/oliver/APEX/replays/TrimAsJW_ROOT/
mv $tempdir/../apex_Trim_$RUN_NUMBER.root $OUTPUT_DIR
mv $tempdir/log/$RUN_NUMBER.log /w/work2/home/oliver/APEX/replays/logs/
mv $tempdir/../apex_$RUN_NUMBER*.dat* $tempdir

echo "moved apex_Trim_$RUN_NUMBER to $OUTPUT_DIR"

rm -f $tempdir/apex_$RUN_NUMBER*.dat*
rmdir $tempdir/log
rmdir $tempdir





