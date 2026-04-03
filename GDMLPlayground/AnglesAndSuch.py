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
# EXAMPLE: <physvol name="PV_AlCap3">  in Rebirth3
# angles = np.array([0, 40, -120]) xyz
# output: [ -36.00521482   18.74723725 -113.8586548 ] XYZ
# Geant4 transforms these object not by rotating them about their local axes,
# but by coordinate transforming, so you must invert the sign of each rotation,
# which is why the rotation for this cap is:
# <rotation name="R-AlCap3" unit="deg" x="36.00521482" y="-18.74723725" z="113.8586548"/>

# Works in reverse as well if you swap xyz and XYZ
# angles = np.array([-36.00521482, 18.74723725, -113.8586548]), XYZ
# output: [ 0.   40. -120.], xyz
# ====================================================================

def int2ext(angles, order='xyz'):
    r_int = R.from_euler(order, angles, degrees=True)
    r_ext = R.from_matrix(r_int.as_matrix())
    ext_angles = r_ext.as_euler('XYZ', degrees=True)
    print("Extrinsic XYZ angles:", np.round(ext_angles, decimals = 8))

def ext2int(angles, order="XYZ"):
    r_ext = R.from_euler(order, angles, degrees=True)
    r_int = R.from_matrix(r_ext.as_matrix())
    int_angles = r_int.as_euler('xyz', degrees=True)
    print("Intrinsic XYZ angles:", np.round(int_angles, decimals = 8))


def rotation_from_vectors(a, b):
    rot, _ = R.align_vectors([b], [a])
    return rot

ApertureDist = 200.34505970786
r1 = rotation_from_vectors(np.array([.373, -0.646, -0.665]), np.array([0.383, -0.6634, -0.6428]))
print("matrix:\n", r1.as_matrix())
print("euler xyz:", r1.as_euler('xyz'))

int2ext(np.array([0, -67.24, -120]))