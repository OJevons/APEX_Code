#!/bin/tcsh
#PBS -N APEX_mass_50k
#PBS -m abe
#PBS -M oliver.jevons@glasgow.ac.uk
#PBS -l walltime=24:00:00
#PBS -l nodes=1
#PBS -V

#######################################################
# Script to run APEX Mass check
# wrapper script
#######################################################

# For testing single runs
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
mkdir -p rootfiles
mkdir -p output
mkdir -p root-output
mkdir -p log

#cp /w/work5/jlab/halla/APEX/trimROOT/apex_Trim_$RUN_NUMBER*.root rootfiles
#cp /w/work2/home/oliver/APEX/replays/50kReplay_LVDCgap0/apex_Trim_$RUN_NUMBER*.root rootfiles
cp /w/work2/home/oliver/APEX/replays/TrimAsJW_ROOT/apex_Trim_$RUN_NUMBER*.root rootfiles
#cp /scratch/oliver/apex_Trim_$RUN_NUMBER*.root rootfiles # FOR TESTING SINGLE RUN
cp /home/oliver/APEX/APEX_Code/invmass/mass_files_C.so ./
cp /home/oliver/APEX/APEX_Code/invmass/mass_files_C_ACLiC_dict_rdict.pcm ./

echo "Files copied; running job"

#nohup analyzer -b -q -l "/home/oliver/APEX/Batch_scripts/inv_mass/batch_mass_scan.C("$RUN_NUMBER")" >& $tempdir/log/$RUN_NUMBER.log
nohup analyzer -b -q -l "/home/oliver/APEX/APEX_Code/invmass/batch_mass_scan.C("$RUN_NUMBER")" >& $tempdir/log/$RUN_NUMBER.log
#nohup analyzer -b -q -l "/home/oliver/APEX/APEX_Code/invmass/batch_mass_scan.C("$RUN_NUMBER")"

echo "executed batch_mass_scan.C  for $RUN_NUMBER"

mv output/Unbinned_mass_$RUN_NUMBER.txt /w/work2/home/oliver/APEX/invmass/TrimAsJW/output/
mv root-output/mass_$RUN_NUMBER.root /w/work2/home/oliver/APEX/invmass/TrimAsJW/root-output/
mv log/$RUN_NUMBER.log /w/work2/home/oliver/APEX/invmass/logs/

echo "moved Unbinned_mass_$RUN_NUMBER.txt to /w/work2/home/oliver/APEX/invmass/TrimAsJW"

rm -f $tempdir/output/Unbinned_mass_$RUN_NUMBER.txt
rm -f $tempdir/root-output/mass_$RUN_NUMBER.root
rm -f $tempdir/rootfiles/apex_Trim_$RUN_NUMBER*.root
rmdir $tempdir/output
rmdir $tempdir/rootfiles
rmdir $tempdir/root-output
rmdir $tempdir/log
rm -f $tempdir/mass_files_C.so
rm -f $tempdir/mass_files_C_ACLiC_dict_rdict.pcm
rmdir $tempdir




