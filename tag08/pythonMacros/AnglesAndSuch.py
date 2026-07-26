import numpy as np
from scipy.spatial.transform import Rotation as R

# Use of this file: In TheRebirth, when defining geometry primitives likes tubes and whatnot, I can initialize 
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

# ====================================================================
# EXAMPLE: <physvol name="PV_Aperture2"> in TheRebirth
# angles = np.array([50, 0, -60]) xyz
# output: [ 30.78973303 -41.56076257 -48.06989481] XYZ

# Works in reverse as well if you swap xyz and XYZ
# angles = np.array([30.78973303, -41.56076257, -48.06989481]), XYZ
# output: [ 50.   0. -60.], xyz
# ====================================================================

def ext2int(angles):
    r_ext = R.from_euler('XYZ', angles, degrees=True)
    r_int = R.from_matrix(r_ext.as_matrix())
    int_angles = r_int.as_euler('xyz', degrees=True)
    print("Intrinsic XYZ angles:", np.round(int_angles, decimals = 8))

def int2ext(angles):
    r_int = R.from_euler('xyz', angles, degrees=True)
    r_ext = R.from_matrix(r_int.as_matrix())
    ext_angles = r_ext.as_euler('XYZ', degrees=True)
    print("Extrinsic XYZ angles:", np.round(ext_angles, decimals = 8))


def rotation_from_vectors(a, b):
    rot, _ = R.align_vectors([b], [a])
    return rot

r1 = rotation_from_vectors(200.34505970786*np.array([-.766,0,.6428]), np.array([-176.242,0,80.244]))

print("matrix:\n", r1.as_matrix())
print("euler xyz:", r1.as_euler('xyz'))