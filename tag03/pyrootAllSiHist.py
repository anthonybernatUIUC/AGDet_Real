import ROOT as root
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.pyplot import cm
from matplotlib.backends.backend_pdf import PdfPages
import os

def plotRootBranch(p, arr, n, **kwargs):
    cm = kwargs.get("color")
    fig = plt.figure(p + 1)
    counts, bins, patches = plt.hist(arr, bins = 500, color = cm[p])
    plt.title(f"Thickness: {round(106*(1 + 0.2*p), 2)}nm")
    plt.xlim(0, 2.5)
    plt.xlabel("MeV")
    plt.ylabel("Counts")
    plt.yscale("log")

    approxPeakEdge = 1.4775
    closestPeakEdgeIndex = np.abs(bins - approxPeakEdge).argmin()
    closestBinIndex = counts[:closestPeakEdgeIndex].argmax()
    halfMax = counts[closestBinIndex] / 2
    closestHalfMaxBinIndex = np.abs(counts[:closestBinIndex] - halfMax).argmin()
    fwhm = bins[closestBinIndex] - bins[closestHalfMaxBinIndex]
    # fwhmArr[p] = fwhm

    leftEdgeIndex = (counts > 10).argmax()
    rangeLen = bins[closestBinIndex] - bins[leftEdgeIndex]
    # rangeArr[p] = rangeLen
    plt.scatter(bins[closestBinIndex], counts[closestBinIndex], color = "red")
    plt.scatter(bins[closestHalfMaxBinIndex], counts[closestHalfMaxBinIndex], color = "red")
    plt.scatter(bins[leftEdgeIndex], counts[leftEdgeIndex], color = "green")
    plt.close()

    plt.hist(arr, bins = 500, zorder = n - p + 1, color = cm[p], label = f"{round(106*(1 + 0.2*p), 2)}nm")
    plt.tick_params(labelsize = 16)
    plt.xlim(0, 2.5)
    plt.xlabel("MeV", fontsize = 20)
    plt.ylabel("Counts", fontsize = 20)
    plt.yscale("log")

    return fig, fwhm, rangeLen


def savePlots(figArr):
    with PdfPages('pyrootHists.pdf') as pdf:
        for fig in figArr:
            pdf.savefig(fig, bbox_inches = 'tight')

def testAllHist():

    cwd = os.getcwd() 
    contents = os.listdir(cwd) 
    rootcontents = [val for val in contents if val.endswith(".root")]
    print("Current directory:", cwd)
    print("Contents:", contents)
    print("Root Files:", sorted(rootcontents))

    numFiles = len(rootcontents)
    fwhmArr = np.zeros(numFiles)
    rangeArr = np.zeros(numFiles)
    color = cm.inferno(np.linspace(0, 1, numFiles))
    figArr = []
    combinedFig = plt.figure(100)
    figArr.append(combinedFig)
    for p, filename in enumerate(sorted(rootcontents)):
        with root.TFile(filename, "READ") as file:
            # print([file.GetListOfKeys().At(i).GetName() for i in range(len(file.GetListOfKeys()))])
            tree = file.Get("AlphaE")
            n = tree.GetEntries()
            try:
                siEdep = np.array([entry.fTotalE for entry in tree])
                fig, fwhm, rangeLen = plotRootBranch(p, siEdep, numFiles, color = color)
                figArr.append(fig)
                fwhmArr[p] = fwhm
                rangeArr[p] = rangeLen

            except Exception as e:
                print("empty tree or error", "\n", e)

    plt.legend(bbox_to_anchor = (1, 1), title = "Target Thickness", fontsize = 20, title_fontsize = 20)    

    widthFig = plt.figure(p + 2)
    figArr.append(widthFig)
    plt.plot(106 * np.linspace(1, 3, numFiles), fwhmArr, label = "FWHM")
    plt.plot(106 * np.linspace(1, 3, numFiles), rangeArr, label = "Peak Range")
    plt.title("Alpha Peak Broadening")
    plt.xlabel("Target Thickness (nm)")
    plt.ylabel("FWHM | Range (MeV)")
    plt.legend()
    plt.close()
    plt.show()
    
    savePlots(figArr)
    
            
testAllHist()
