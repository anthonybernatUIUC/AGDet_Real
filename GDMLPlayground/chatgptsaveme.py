import numpy as np

def deg2rad(deg): return deg * np.pi / 180
def rad2deg(rad): return rad * 180 / np.pi

def z_axis_alignment(v):
    """
    Compute X and Y rotations (intrinsic XYZ) to align local Z-axis with vector v.
    Returns angles in degrees: x, y, z=0
    """
    v = np.array(v, dtype=float)
    v /= np.linalg.norm(v)  # normalize

    vx, vy, vz = v
    y = np.arcsin(-vx)
    x = np.arctan2(vy, vz)
    z = 0.0  # irrelevant for Z alignment
    return rad2deg(x), rad2deg(y), rad2deg(z)

ApertureDist = 200.34505970786
mm = 1

target_vector = ApertureDist*np.array([0.76604444, 0, 0.64278761]) - np.array([9.025, 0, 0]) # your vector
# target_vector = ApertureDist*np.array([0.766, 0, 0.6428]) - np.array([9.351, 0, 0.140]) # your vector
x_rot, y_rot, z_rot = z_axis_alignment(target_vector)
print(f"Rotate x={x_rot:.6f}, y={y_rot:.6f}, z={z_rot:.6f}")

target_vector = ApertureDist*np.array([-0.383, 0.6634, 0.6428]) - np.array([-4.512, 0, -7.815]) # your vector
# target_vector = ApertureDist*np.array([-0.383, 0.6634, 0.6428]) - np.array([-5.385*mm, 0, -6.070*mm]) # your vector
x_rot, y_rot, z_rot = z_axis_alignment(target_vector)
print(f"Rotate x={x_rot:.6f}, y={y_rot:.6f}, z={z_rot:.6f}")

target_vector = ApertureDist*np.array([-0.383, -0.6634, 0.6428]) - np.array([-4.512, 0, 7.815]) # your vector
# target_vector = ApertureDist*np.array([-0.383, -0.6634, 0.6428]) - np.array([-4.1*mm, 0, 7.939*mm]) # your vector
x_rot, y_rot, z_rot = z_axis_alignment(target_vector)
print(f"Rotate x={x_rot:.6f}, y={y_rot:.6f}, z={z_rot:.6f}")

target_vector = ApertureDist*np.array([-0.92215, 0, 0.38685]) - np.array([-14.493, 0, 0]) # your vector
# target_vector = ApertureDist*np.array([-0.92215, 0, 0.38685]) - np.array([-14.365*mm, 0, -0.203*mm]) # your vector
x_rot, y_rot, z_rot = z_axis_alignment(target_vector)
print(f"Rotate x={x_rot:.6f}, y={y_rot:.6f}, z={z_rot:.6f}")

target_vector = ApertureDist*np.array([0.4611, 0.7986, 0.3869]) - np.array([7.247, 0, -12.552]) # your vector
# target_vector = ApertureDist*np.array([0.4611, 0.7986, 0.3869]) - np.array([3.343*mm, 0, -12.072*mm]) # your vector
x_rot, y_rot, z_rot = z_axis_alignment(target_vector)
print(f"Rotate x={x_rot:.6f}, y={y_rot:.6f}, z={z_rot:.6f}")

target_vector = ApertureDist*np.array([0.4611, -0.7986, 0.3869]) - np.array([7.247, 0, 12.552]) # your vector
# target_vector = ApertureDist*np.array([0.4611, -0.7986, 0.3869]) - np.array([8.321*mm, 0, 12.449*mm]) # your vector
x_rot, y_rot, z_rot = z_axis_alignment(target_vector)
print(f"Rotate x={x_rot:.6f}, y={y_rot:.6f}, z={z_rot:.6f}")
