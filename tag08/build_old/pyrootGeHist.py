import ROOT as root
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.pyplot import cm
import matplotlib.ticker as tck
from matplotlib.backends.backend_pdf import PdfPages
import os

def plotRootBranch(p, arr, n, **kwargs):

    cm = kwargs.get("color")
    el = kwargs.get("element")
    sep = kwargs.get("separate", False)

    if sep:
        fig = plt.figure(p + 1, figsize=(100, 6))

    if (el == ""): plt.hist(arr, bins = 6000, zorder = -1, color = "black", label = f"Mixed Elements")
    else: plt.hist(arr, bins = 6000, zorder = n - p + 1, color = cm[p], label = f"Element: {el}")
    
    plt.xlim(0, 3.0)
    plt.ylim(1, 1e4)
    plt.xlabel("MeV", fontsize = 16)
    plt.ylabel("Counts", fontsize = 16)
    plt.yscale("log")
    if sep:
        ax = plt.gca() 
        ax.xaxis.set_minor_locator(tck.AutoMinorLocator())
        ax.tick_params(axis='both', which='major', length = 12, labelsize = 16)
        ax.tick_params(axis='both', which='minor', length = 8)
        plt.legend(bbox_to_anchor = (1, 1), fontsize = 12)
        # fig.set_size_inches((16, 10))

def testAllHist():

    cwd = os.getcwd() 
    contents = os.listdir(cwd) 
    rootcontents = [val for val in contents if val.endswith(".root") and val.startswith("AGMACRO")]
    print("Root Files:", sorted(rootcontents))

    numFiles = len(rootcontents)
    color = cm.plasma(np.linspace(0, 1, numFiles))
    for p, filename in enumerate(sorted(rootcontents)):
        with root.TFile(filename, "READ") as file:
            tree = file.Get(file.GetListOfKeys().At(0).GetName())
            try:
                plotRootBranch(p, [entry.fEdepGe for entry in tree], numFiles, color = color, 
                               element = filename.split("AGMACRO")[1].split(".root")[0])

            except Exception as e:
                print("empty tree or error", "\n", e)
    
    ax = plt.gca()
    ax.xaxis.set_minor_locator(tck.AutoMinorLocator())
    ax.tick_params(axis='both', which='major', length = 12, labelsize = 16)
    ax.tick_params(axis='both', which='minor', length = 6)
    plt.show()

def testAllHist2():

    treeName = "GeScoring"
    columnName = "fEdepGe"

    cwd = os.getcwd() 
    contents = os.listdir(cwd) 
    rootcontents = sorted([val for val in contents if val.endswith(".root") and val.startswith("AGMACRO")])

    numFiles = len(rootcontents)
    color = cm.plasma(np.linspace(0, 1, numFiles))
    for p, filename in enumerate(rootcontents):
        plotRootBranch(p, root.RDataFrame(treeName, rootcontents[p]).AsNumpy([columnName, ])[columnName], 
                       numFiles, color = color, element = filename.split("AGMACRO")[1].split(".root")[0], 
                       separate = False)
    
    ax = plt.gca() 
    ax.xaxis.set_minor_locator(tck.AutoMinorLocator())
    ax.tick_params(axis='both', which='major', length = 12, labelsize = 16)
    ax.tick_params(axis='both', which='minor', length = 8)
    plt.legend(bbox_to_anchor = (1, 1), fontsize = 12)
    plt.show()

def testAllHist3():
    c1 = root.TCanvas()
    c1.SetLogy(1)
    df = root.RDataFrame("GeScoring", "AGMACRO.root")
    h = df.Histo1D(("uhh", "Simulated Energy Deposition in Germanium Detector", 3000, 0, 3), "fEdepGe")
    h.SetStats(0)
    h.SetLineColorAlpha(632, 0.35)
    h.SetFillStyle(3002)
    h.SetFillColor(632)
    h.Draw()
    input("Press enter to exit")

# testAllHist()
testAllHist2()
# testAllHist3() 