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


Mass_H_L = list(range(125,225,1)) #235, 125,225 -> JohnW final details
#Mass_H_L = list(range(145,150,1)) + list(range(180,225,1))
#Mass_H_L = list(range(160,180,1)) + list(range(200,225,1))
#Mass_H_L = list(range(225,235,1))
#Mass_H_L = list(range(150,160,5)) # Testing

Mass_dec = [0.25,0.50,0.75] # numbers after decimal place

Mass_list = []

for Mass_H in Mass_H_L:
    Mass_list.append(Mass_H)
    for dec in Mass_dec:
        Mass_list.append("{:.2f}".format(Mass_H+dec))

        


PO_L = [3] #[3,5]
TPO = -1 # toy model order

WF_L = list(range(5,25))
#WF_L = list(range(9,10)) # Testing

WF5_L = list(range(5,76)) # window factors for order 5 at certain masses
#WF5_L = list(range(20,30,5)) # Testing

MCutoff = 160 # above this mass for 5th order use greater window sizes


NBg = 10000 # number of background pars
NSig = 0
MT = 3


# read 3rd and 5th order Window factors from csv
# p3pars = []
# p5pars = []


# fname = "csv/WinSize/WinFunc.csv"
# with open(fname) as csvfile:
#     csvreader = csv.DictReader(csvfile)
#     for row in csvreader:
#         print(f"Order = row['Order'], P0 = {row['P0']}, P1 = {row['P1']}, P2 = {row['P2']}, P3 = {row['P3']}, P4 = {row['P4']}")
        
#         if row['Order'] == '3':
#             p3pars = [float(row['P0']),float(row['P1']),float(row['P2']),float(row['P3']),float(row['P4'])]
#         if row['Order'] == '5':
#             p5pars = [float(row['P0']),float(row['P1']),float(row['P2']),float(row['P3']),float(row['P4'])]


# print(f"p3pars = {p3pars}")
# print(f"p5pars = {p5pars}")
# testRes3 = calcWF(200,p3pars)
# testRes5 = calcWF(200,p5pars)
            
# print(f"calcWF result: p3 = {testRes3}, p5 = {testRes5}")



#updated
# for Mass_H in Mass_H_L:
#     for PO in PO_L:


        

#         TPO = PO + 2
#         if PO == 3:
#             WF_M = calcWF(Mass_H,p3pars) # estimate of largest window needed
#             for WF in range(WF_M[0],WF_M[1]):
#                 rows.append([Mass_H,PO,TPO,WF,NBg,NSig,MT])

#         if PO == 5:
#             WF_M = calcWF(Mass_H,p5pars) # estimate of largest window needed
#             for WF in range(WF_M[0],WF_M[1]):                
#                 rows.append([Mass_H,PO,TPO,WF,NBg,NSig,MT])


        # TPO = PO + 2
        # if PO == 3:
        #     WF_M = int(calcWF(Mass_H,p3pars)) # estimate of largest window needed
        #     for WF in range(WF_M-12,WF_M+3):
        #         if WF>4:
        #             rows.append([Mass_H,PO,TPO,WF,NBg,NSig,MT])

        # if PO == 5:
        #     WF_M = int(calcWF(Mass_H,p5pars)) # estimate of largest window needed
        #     for WF in range(WF_M-12,WF_M+3):
        #         if WF>4:
        #             rows.append([Mass_H,PO,TPO,WF,NBg,NSig,MT])


                

        


for Mass_H in Mass_list:
    print(f"new mass = {Mass_H}")
 
    
    for PO in PO_L:

        if PO>3 and Mass_H>MCutoff:                
            for WF in WF5_L:
                TPO = PO
                rows.append([Mass_H,PO,TPO,WF,NBg,NSig,MT])
        else:
            for WF in WF_L:
                TPO = PO
                rows.append([Mass_H,PO,TPO,WF,NBg,NSig,MT])
        


#rows.append(["140","5","5"])



with open('mSearchParams/mass_params_full.csv', 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerows(rows)
