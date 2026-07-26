from sklearn import tree

import ROOT as root
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.pyplot import cm
from matplotlib.backends.backend_pdf import PdfPages
import os

rootFiles = np.array([])
bessels = [ (0, 1), (0, 2), (0, 3),
            (1, 1), (1, 2), (1, 3),
            (2, 1), (2, 2), (2, 3),
            (3, 1), (3, 2), (3, 3) ]
thickness = ["thin", "thick"]

count = 0
path = "/home/anthony/software/AGDet_Real/tag08/rootFiles/cone6cm"
for i in bessels:
    for j in thickness:
        filenum = f"{count}"
        if count < 10: filenum = "0" + filenum
        filename = f"cone_{j}_{i[0]}_{i[1]}.root"
        rootFiles = np.append(rootFiles, f"{path}/{filename}")
        count += 1

gammaThinPhotoCounts = []
gammaThickPhotoCounts = []
alphaThinPhotoCounts = []
alphaThickPhotoCounts = []

branch_name = "fEdepGamma"
min_val = 0.47
max_val = 0.48
for p, filename in enumerate(rootFiles):
    if "thin" in filename:
        with root.TFile.Open(filename, "READ") as file:
            detCounts = []
            detCountsAlpha = []
            for i in range(1,7):
                bleh = file.Get(f"GammaDet{i}")
                photocounts = bleh.GetEntries(f"{branch_name} >= {min_val} && {branch_name} <= {max_val}")
                detCounts.append(photocounts)

                bleh2 = file.Get(f"AlphaDet{i}")
                counts = bleh2.GetEntries()
                detCountsAlpha.append(counts)
                
            gammaThinPhotoCounts.append(detCounts)
            alphaThinPhotoCounts.append(detCountsAlpha)
            
    else:
        with root.TFile.Open(filename, "READ") as file:
            detCounts = []
            detCountsAlpha = []
            for i in range(1,7):
                bleh = file.Get(f"GammaDet{i}")
                photocounts = bleh.GetEntries(f"{branch_name} >= {min_val} && {branch_name} <= {max_val}")
                detCounts.append(photocounts)

                bleh2 = file.Get(f"AlphaDet{i}")
                counts = bleh2.GetEntries()
                detCountsAlpha.append(counts)

            gammaThickPhotoCounts.append(detCounts)
            alphaThickPhotoCounts.append(detCountsAlpha)

gammaThinPhotoCounts = np.array(gammaThinPhotoCounts)
gammaThickPhotoCounts = np.array(gammaThickPhotoCounts)
alphaThinPhotoCounts = np.array(alphaThinPhotoCounts)
alphaThickPhotoCounts = np.array(alphaThickPhotoCounts)

colors = ["blue", "blue", "blue", "red", "red", "red"]
fig, axs = plt.subplots(3, 4, figsize = (12, 8))
for i, row in enumerate(gammaThinPhotoCounts):
    axs[i%3, i//3].bar(np.linspace(1,6,6), row, yerr = np.sqrt(row), color = colors)
    axs[i%3, i//3].set_title(f"Thin Bessel {bessels[i]}")
    axs[i%3, i//3].set_xlabel("Detector Number")
    axs[i%3, i//3].set_ylabel("Counts")
    axs[i%3, i//3].set_ylim(.999*min(row), 1.001*max(row))
    axs[i%3, i//3].set_xticks(np.linspace(1,6,6))
fig.tight_layout()

fig2, axs2 = plt.subplots(3, 4, figsize = (12, 8))
for i, row in enumerate(gammaThickPhotoCounts):
    axs2[i%3, i//3].bar(np.linspace(1,6,6), row, yerr = np.sqrt(row), color = colors)
    axs2[i%3, i//3].set_title(f"Thick Bessel {bessels[i]}")
    axs2[i%3, i//3].set_xlabel("Detector Number")
    axs2[i%3, i//3].set_ylabel("Counts")
    axs2[i%3, i//3].set_ylim(.999*min(row), 1.001*max(row))
    axs2[i%3, i//3].set_xticks(np.linspace(1,6,6))
fig2.tight_layout()

SiDetNames = ["MR", "TL", "BL", "ML", "TR", "BR"]
SiColors = ["blue", "red", "red", "red", "blue", "blue"]

fig3, axs3 = plt.subplots(3, 4, figsize = (12, 8))
for i, row in enumerate(alphaThinPhotoCounts):
    axs3[i%3, i//3].bar(SiDetNames, row, yerr = np.sqrt(row), color = SiColors)
    axs3[i%3, i//3].set_title(f"Thin Bessel {bessels[i]}")
    axs3[i%3, i//3].set_xlabel("Alpha Detector")
    axs3[i%3, i//3].set_ylabel("Counts")
    axs3[i%3, i//3].set_ylim(.999*min(row), 1.001*max(row))
fig3.tight_layout()

cats = ["ThinGamma", "ThickGamma", "ThinAlpha"]
sums = np.array([np.sum(gammaThinPhotoCounts, axis = 1), 
                 np.sum(gammaThickPhotoCounts, axis = 1), 
                 np.sum(alphaThinPhotoCounts, axis = 1)])
fig4, axs4 = plt.subplots(1, 3, figsize = (12, 8))
for i, sum in enumerate(sums):
    print(np.sqrt(sum))
    axs4[i].bar([str(b) for b in bessels], sum, yerr = np.sqrt(sum))
    axs4[i].set_title(f"{cats[i]}")
    axs4[i].set_ylabel("Total Counts")
fig4.tight_layout()

plt.show()