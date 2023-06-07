#!/usr/bin/python

#######################################################
# Generate csv with list job parameters
# Author: John Williamson
# Created: 7th December 2021
#######################################################

# example use:
# python gen_joblist.py

import csv


# calculate max window size to use
#def calcWF(mass, a, b, c, d, e):
def calcWF(mass, pars):
    i = 0
    WF = 0
    for par in pars:
        WF += int(par*(mass**i))
        i += 1

        
    WF_L = 0 # low value
    WF_H = 0 # high value

    Low = 12 # how many window factors lower to take
    High = 4 # how many window factors greater to take
    Lim = 17 # where WF is small so adjust low and high
    # test if WF is small (in which case extend range to higher orders)
    if WF < Lim:
        WF_L = 5
        WF_H = Lim +  High
    else:
        WF_L = WF - Low
        WF_H = WF + High

    Res = [WF_L,WF_H]
        
    return Res


rows = [["MASS_H","PO","TPO","WF","NBg","NSig","MT"]]


Mass_H_L = list(range(125,225,1))

Mass_dec = [0.25,0.50,0.75] # numbers after decimal place

Mass_list = []

for Mass_H in Mass_H_L:
    Mass_list.append(Mass_H)
    for dec in Mass_dec:
        Mass_list.append("{:.2f}".format(Mass_H+dec))
      

#PO_L = [3] #[3,5]
PO = 3 
TPO = -1 # toy model order

WF_L = list(range(5,25))

NBg = 10000 # number of background pars
NSig = 0
MT = 3        

MCutoff1 = 144
MCutoff2 = 165
MCutoff3 = 216

for Mass_H in Mass_list:
    print(f"new mass = {Mass_H}")
    
    if Mass_H<MCutoff1:
        #TPO = PO
        rows.append([Mass_H,PO,TPO,9,NBg,NSig,MT])
    if Mass_H>=MCutoff1 and Mass_H<MCutoff2:
        #TPO = PO
        rows.append([Mass_H,PO,TPO,11,NBg,NSig,MT])
    if Mass_H>=MCutoff2 and Mass_H<MCutoff3:
        #TPO = PO
        rows.append([Mass_H,PO,TPO,12,NBg,NSig,MT])
    else:
        #TPO = PO
        rows.append([Mass_H,PO,TPO,12,NBg,NSig,MT])        


with open('mSearchParams/mass_params_BlindChosen.csv', 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(rows)
