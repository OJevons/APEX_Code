#!/usr/bin/python

#######################################################
# Submit mass search jobs
# Author: John Williamson
# Created: 8th December 2021
#######################################################

# example use:
# python mass_submitter.py example.csv

import os
import subprocess
import sys
import csv
import time

fname = sys.argv[1] # filename to get all mass search params
#massDir = sys.argv[2] # directory to check which have been performed

# os.environ['NBg'] = '1000'
# os.environ['NSig'] = '0'
# os.environ['PO'] = '5'
# os.environ['MT'] = '3'
# os.environ['MASS_H'] = '130'
# os.environ['WF'] = '130'

#os.system('qsub run_mass_search.sh')

#joblim = 50
joblim = 150
username = 'oliver'
smallSleep = 3
largeSleep = 10

os.system('echo "Running peak search code"')

with open(fname) as csvfile:
    csvreader = csv.DictReader(csvfile)
    for row in csvreader:
        os.environ['NBg'] = row['NBg']
        os.environ['NSig'] = row['NSig']
        os.environ['PO'] = row['PO']
        os.environ['TPO'] = row['TPO']
        os.environ['MT'] = row['MT']
        os.environ['MASS_H'] = row['MASS_H']
        os.environ['WF'] = row['WF']
        
        os.system('echo "NBg = $NBg"')
        os.system('echo "NSig = $NSig"')
        os.system('echo "PO = $PO"')
        os.system('echo "TPO = $TPO"')
        os.system('echo "MT = $MT"')
        os.system('echo "MASS_H = $MASS_H"')
        os.system('echo "WF = $WF"')

        count = 0
        while(int(os.popen(f"qselect -u {username} | wc -l").read()) >= joblim):
            time.sleep(largeSleep)
            # print("too many jobs")
            count += 1
            if(count>2):
                print("waiting for jobs in queue to finish before submitting more")
                print(f"currently {os.popen(f'qselect -u {username} | wc -l').read()} jobs in queue")
                count = 0
                time.sleep(largeSleep)
            

        #os.system('qsub run_mass_search.sh')
        os.system('qsub -o /dev/null -e /dev/null run_mass_search.sh')
        #os.system('./run_mass_search.sh')
        print(" ")
        time.sleep(smallSleep)
    # os.environ['NBg'] = '1000'

    
#    os.system(execute)
