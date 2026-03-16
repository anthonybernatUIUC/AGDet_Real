import numpy as np
from scipy.spatial.transform import Rotation as R

# Use of this file: In my TheRebirth, when defining geometry primitives likes tubes and whatnot, I can initialize 
# a rotation about its intrinsic, or local, origin. However, when placing composite geometry, like when I take
# boolean operations on primitives, the XYZ rotations are about the global origin, so to align the normals of
# primitives to where they belong in the world, I must convert the angles.

angles = np.array([50, 0, -60])

# "XYZ" extrinsic, "xyz" intrinsic
r_intrinsic = R.from_euler('xyz', angles, degrees=True)

# Equivalent to intrinsic XYZ == extrinsic ZYX (reverse order)
r_extrinsic = R.from_matrix(r_intrinsic.as_matrix())
extrinsic_angles = r_extrinsic.as_euler('XYZ', degrees=True)

print("Extrinsic XYZ angles:", np.round(extrinsic_angles, decimals = 8))

# EXAMPLE: <physvol name="PV_Aperture2"> in TheRebirth
# angles = np.array([50, 0, -60]) xyz
# output: [ 30.78973303 -41.56076257 -48.06989481] XYZ

# Works in reverse as well if you swap xyz and XYZ
# angles = np.array([30.78973303, -41.56076257, -48.06989481]), XYZ
# output: [ 50.   0. -60.], xyz

