#!/usr/bin/python

#######################################################
# Generate plots with list job parameters
# Author: John Williamson
# Created: 8th December 2021
#######################################################

# example use:
# python run_plots.py

import csv
import os
import numpy

# rows = [["MASS_H","PO","WF","NBg","NSig","MT"]]

#Mass_H_L = list(range(125.0,225.0,0.25)) # Actual list of masses - from John
Mass_H_L = list(numpy.arange(125,225,0.25)) # Actual list of masses - from John

PO_L = [3]

WF_L = list(range(5,46))

WF5_L = list(range(5,76)) # window factors for order 5 at certain masses

MCutoff = 160 # above this mass for 5th order use greater window sizes

# Mass_H_L = [135]
# PO_L = [3]
# WF_L = [5]

NBg = 1000
NSig = 0
MT = 3

for Mass_H in Mass_H_L:
    for PO in PO_L:
        #TPO = PO + 2
        TPO = PO
        os.system(f'analyzer -b -q -l "convert_csv.C({Mass_H},{PO},{TPO},{NSig},{MT},0.15)"')
        os.system('echo NOW RUNNING PLOT_PVALS.C')
        os.system(f'analyzer -b -q -l "plot_pvals.C({Mass_H},{PO},{TPO},{NSig},{MT},0.15)"')
        os.system('echo NOW RUNNING PLOT_LIMS.C')
        os.system(f'analyzer -b -q -l "plot_lims.C({Mass_H},{PO},{TPO},{NSig},{MT},0.15)"')


os.system(f'convert plots/Chi2/0.15*3p* plots/Chi2/comb/0.15_3p_Chi2_all_masses.pdf')
os.system(f'convert plots/Chi2/0.15*5p* plots/Chi2/comb/0.15_5p_Chi2_all_masses.pdf')
os.system(f'convert plots/Chi2/0.15* plots/Chi2/comb/0.15_Chi2_all_masses.pdf')

os.system(f'convert plots/Pulls/0.15*3p* plots/Pulls/comb/0.15_3p_Pulls_all_masses.pdf')
os.system(f'convert plots/Pulls/0.15*5p* plots/Pulls/comb/0.15_5p_Pulls_all_masses.pdf')
os.system(f'convert plots/Pulls/0.15* plots/Pulls/comb/0.15_Pulls_all_masses.pdf')

os.system(f'convert plots/MassScan/0.15*3p* plots/MassScan/comb/0.15_3p_MassScan_all_masses.pdf')
os.system(f'convert plots/MassScan/0.15*5p* plots/MassScan/comb/0.15_5p_MassScan_all_masses.pdf')
os.system(f'convert plots/MassScan/0.15* plots/MassScan/comb/0.15_MassScan_all_masses.pdf')


 # MassScan
 # Pulls
 
        
#rows.append(["140","5","5"])



