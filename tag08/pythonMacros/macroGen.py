import numpy as np

def mTargetWidth():
    file = open("genSim.mac", "w")
    file.write("/control/cout/ignoreThreadsExcept 0" + "\n")
    file.write("/control/verbose 1\n")
    file.write("/run/verbose 1\n\n")
    file.write("/tracking/verbose 0\n\n")
    
    printProgress = 200000
    beamOn = 100000000
    minWidthScale = 1
    maxWidthScale = 3
    step = .2
    scales = np.arange(minWidthScale, maxWidthScale + step, step)
    print(scales)

    for i, s in enumerate(scales):
        filenum = f"{i}"
        if i < 10: filenum = "0" + filenum
        file.write(f"/target/setWidth {round(s, 2)}\n")
        file.write(f"/analysis/setFileName TestBKG{filenum}\n")
        file.write(f"/run/printProgress {printProgress}\n")
        file.write(f"/run/initialize\n")
        file.write(f"/run/reinitializeGeometry\n")
        file.write(f"/run/beamOn {beamOn}\n\n")

    
def gettingTilty():
    file = open("genSim.mac", "w")
    file.write("/control/cout/ignoreThreadsExcept 0\n")
    file.write("/control/verbose 0\n")
    file.write("/run/verbose 0\n\n")
    file.write("/tracking/verbose 0\n\n")

    printProgress = 200000
    beamOn = 10000000
    gunModes = ["default", "left", "right", "uniformArea"]
    apertureShifts = [-400, -100, 0, 100, 400]

    count = 0
    for i in apertureShifts:
        for j in gunModes:
            filenum = f"{count}"
            if count < 10: filenum = "0" + filenum
            file.write(f"/aperture/shiftAperture {i}\n")
            file.write(f"/gun/setMode {j}\n")
            file.write(f"/analysis/setFileName AGMACROConeDist{filenum}\n")
            file.write(f"/run/printProgress {printProgress}\n")
            file.write(f"/run/initialize\n")
            file.write(f"/run/reinitializeGeometry\n")
            file.write(f"/run/beamOn {beamOn}\n")
            file.write(f"/aperture/shiftAperture {-i}\n\n")
            count += 1

def CountsDracula():
    file = open("genSim.mac", "w")
    file.write("/control/cout/ignoreThreadsExcept 0\n")
    file.write("/control/verbose 0\n")
    file.write("/run/verbose 0\n\n")
    file.write("/tracking/verbose 0\n\n")

    printProgress = 200
    beamOn = 10000
    bessels = [(0, 1), (0, 2), (0, 3),
               (1, 1), (1, 2), (1, 3),
               (2, 1), (2, 2), (2, 3),
               (3, 1), (3, 2), (3, 3)]
    thickness = ["thin", "thick"]

    count = 0
    for i in bessels:
        for j in thickness:
            filenum = f"{count}"
            if count < 10: filenum = "0" + filenum
            file.write(f"/target/setThickness {j}\n")
            file.write(f"/target/setBesselNu {i[0]} {i[1]}\n")
            file.write(f"/analysis/setFileName test_{j}_{i[0]}_{i[1]}\n")
            file.write(f"/run/printProgress {printProgress}\n")
            file.write(f"/run/beamOn {beamOn}\n\n")
            count += 1

    # for i in bessels:
    #     filenum = f"{count}"
    #     if count < 10: filenum = "0" + filenum
    #     file.write(f"/target/setBesselNu {i[0]} {i[1]}\n")
    #     file.write(f"/analysis/setFileName ../rootFiles/AGMACRO_{count}_{i[0]}_{i[1]}\n")
    #     file.write(f"/run/printProgress {printProgress}\n")
    #     file.write(f"/run/beamOn {beamOn}\n\n")
    #     count += 1

CountsDracula()

# /control/cout/ignoreThreadsExcept 0
# /control/verbose 0
# /run/verbose 0
# /tracking/verbose 0

# /shell/setShellType 1
# /run/initialize
# /run/reinitializeGeometry

# /analysis/setFileName AGMACROTest
# /run/printProgress 200000
# /run/initialize
# /run/beamOn 100000000