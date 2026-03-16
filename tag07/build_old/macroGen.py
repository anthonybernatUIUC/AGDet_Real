import numpy as np

def mTargetWidth():
    file = open("genSim.mac", "w")
    file.write("/control/cout/ignoreThreadsExcept 0" + "\n")
    file.write("/control/verbose 1\n")
    file.write("/run/verbose 1\n\n")
    file.write("/tracking/verbose 0\n\n")
    
    printProgress = 200000
    beamOn = 1000000
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

    
mTargetWidth()
    


# /target/setWidth 1.25
# /analysis/setFileName TestBKG2
# /run/printProgress 200000
# /run/initialize
# /run/reinitializeGeometry
# /run/beamOn 1000000