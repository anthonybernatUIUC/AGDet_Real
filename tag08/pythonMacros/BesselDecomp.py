import numpy as np
import pandas as pd
import matplotlib.pyplot as plt 
from scipy.special import jv, jn_zeros
from sklearn.linear_model import Ridge

# df = pd.read_excel("../Blue Ridge LiF Map_2025_09.xlsx", usecols = "A,B,C", skiprows = 4, nrows = 950 - 5)
df = pd.read_excel("../Blue Ridge LiF Map_2025_09.xlsx", usecols = "A,B,C", skiprows = 4, nrows = 963 - 5, sheet_name="LiF 958 point map")
dfnp = df.to_numpy()
X = dfnp[:, 0]
Y = dfnp[:, 1]
Z = dfnp[:, 2]

max_m = 15
max_n = 15

def SetMapping(max_m=15, max_n=15):
    # 1. Setup coordinates
    r = np.sqrt(X**2 + Y**2)
    theta = np.arctan2(Y, X)
    R = r.max()
    
    # 2. Build the matrix
    basis = []
    mapping = []

    for m in range(max_m + 1):
        zeros = jn_zeros(m, max_n)
        for n in range(1, max_n + 1):
            alpha = zeros[n - 1]
            radial_part = jv(m, alpha * r / R) # J_nu(r), normalized to largest radius R
            
            if m == 0:
                basis.append(radial_part)
                mapping.append({'m': m, 'n': n, 'type': 'symmetric'})
            else:
                basis.append(radial_part * np.cos(m * theta))
                mapping.append({'m': m, 'n': n, 'type': 'cos'})
                
                basis.append(radial_part * np.sin(m * theta))
                mapping.append({'m': m, 'n': n, 'type': 'sin'})
    
    A = np.column_stack(basis)
    return mapping, A

mapping, A = SetMapping()

# 2 methods to solve y = Ax + e, where y is Z, A is the basis matrix, x is the coefficients, and e is the error
def solve_bessel_lstsq(mapping, A, Z):
    z_centered = Z - Z.mean()
    coeffs, _, _, _ = np.linalg.lstsq(A, z_centered, rcond=None)
    return (coeffs, mapping)

# Supposedly helps with overfitting (spoiler: it does a lot)
def solve_bessel_ridge(mapping, A, Z):
    z_centered = Z - Z.mean()
    model = Ridge(alpha=1.0, fit_intercept=True) 
    return (model.fit(A, z_centered).coef_, mapping, model.intercept_)


def plotato(mapping, A, Z):

    (coeffs_lstsq, mapping_lstsq) = solve_bessel_lstsq(mapping, A, Z)
    (coeffs_ridge, mapping_ridge, intercept_ridge) = solve_bessel_ridge(mapping, A, Z)

    # Reconstruct 
    z_recons_lstsq = A @ coeffs_lstsq
    z_recons_ridge = A @ coeffs_ridge

    plt.figure(1)
    plt.tricontourf(X, Y, z_recons_lstsq + Z.mean(), levels = 20, cmap='coolwarm')
    plt.colorbar(label='Thickness (nm)')
    plt.title("Lstsq Reconstructed")
    plt.xlim(-4, 4)
    plt.ylim(-4, 4)
    plt.xlabel("X (mm)")
    plt.ylabel("Y (mm)")

    plt.figure(2)
    plt.tricontourf(X, Y, z_recons_ridge + Z.mean() + intercept_ridge, levels = 20, cmap='coolwarm')
    plt.colorbar(label='Thickness (nm)')
    plt.title("Ridge Reconstructed")
    plt.xlim(-4, 4)
    plt.ylim(-4, 4)
    plt.xlabel("X (mm)")
    plt.ylabel("Y (mm)")

    plt.figure(3)
    plt.tricontourf(X, Y, Z, levels = 20, cmap='coolwarm')
    plt.colorbar(label='Thickness (nm)')
    plt.title("Measured")
    plt.xlim(-4, 4)
    plt.ylim(-4, 4)
    plt.xlabel("X (mm)")
    plt.ylabel("Y (mm)")

    errors_lstsq = Z - (z_recons_lstsq + Z.mean())
    plt.figure(4)
    plt.tricontourf(X, Y, errors_lstsq, levels = 20, cmap='coolwarm')
    # plt.scatter(X, Y, c = errors, cmap = 'coolwarm')
    plt.colorbar(label='Prediction Error (nm)')
    plt.title('Measured - Reconstructed (lstsq) ')
    plt.xlim(-4, 4)
    plt.ylim(-4, 4)
    plt.xlabel("X (mm)")
    plt.ylabel("Y (mm)")

    errors_ridge = Z - (z_recons_ridge + Z.mean() + intercept_ridge)
    plt.figure(5)
    plt.tricontourf(X, Y, errors_ridge, levels = 20, cmap='coolwarm')
    # plt.scatter(X, Y, c = errors, cmap = 'coolwarm')
    plt.colorbar(label='Prediction Error (nm)')
    plt.title('Measured - Reconstructed (ridge)')
    plt.xlim(-4, 4)
    plt.ylim(-4, 4)
    plt.xlabel("X (mm)")
    plt.ylabel("Y (mm)")

    plt.show()
# plotato(mapping, A, Z)

def plotato2(mapping, A, Z):

    (coeffs_lstsq, mapping_lstsq) = solve_bessel_lstsq(mapping, A, Z)
    (coeffs_ridge, mapping_ridge, intercept_ridge) = solve_bessel_ridge(mapping, A, Z)

    # Reconstruct 
    z_recons_lstsq = A @ coeffs_lstsq
    z_recons_ridge = A @ coeffs_ridge

    fig, axes = plt.subplots(2, 3, figsize=(18, 10))
    axes = axes.flatten()

    # Plot 0: Measured
    ax = axes[0]
    cont = ax.tricontourf(X, Y, Z, levels=20, cmap='coolwarm')
    fig.colorbar(cont, ax=ax, label='Thickness (nm)')
    ax.set_title("Measured")

    # Plot 1: Lstsq Reconstructed
    ax = axes[1]
    cont = ax.tricontourf(X, Y, z_recons_lstsq + Z.mean(), levels=20, cmap='coolwarm')
    fig.colorbar(cont, ax=ax, label='Thickness (nm)')
    ax.set_title("Lstsq Reconstructed")

    # Plot 2: Ridge Reconstructed
    ax = axes[2]
    cont = ax.tricontourf(X, Y, z_recons_ridge + Z.mean() + intercept_ridge, levels=20, cmap='coolwarm')
    fig.colorbar(cont, ax=ax, label='Thickness (nm)')
    ax.set_title("Ridge Reconstructed")

    # Plot 3: Lstsq Error
    ax = axes[3]
    errors_lstsq = Z - (z_recons_lstsq + Z.mean())
    cont = ax.tricontourf(X, Y, errors_lstsq, levels=20, cmap='coolwarm')
    fig.colorbar(cont, ax=ax, label='Prediction Error (nm)')
    ax.set_title('(Measured - Lstsq)')

    # Plot 4: Ridge Error
    ax = axes[4]
    errors_ridge = Z - (z_recons_ridge + Z.mean() + intercept_ridge)
    cont = ax.tricontourf(X, Y, errors_ridge, levels=20, cmap='coolwarm')
    fig.colorbar(cont, ax=ax, label='Prediction Error (nm)')
    ax.set_title('(Measured - Ridge)')

    # Hide the empty 6th subplot
    axes[5].axis('off')

    # Apply universal settings to all visible plots
    for ax in axes[:5]:
        ax.set_xlim(-4, 4)
        ax.set_ylim(-4, 4)
        ax.set_xlabel("X (cm)")
        ax.set_ylabel("Y (cm)")

    plt.tight_layout()
    plt.show()

plotato2(mapping, A, Z)


(coeffs_lstsq, mapping_lstsq) = solve_bessel_lstsq(mapping, A, Z)
(coeffs_ridge, mapping_ridge, intercept_ridge) = solve_bessel_ridge(mapping, A, Z)

def plot_coefficients(mapping, coeffs, max_m, max_n):

    coeff_strength = np.zeros((max_m + 1, max_n))
    for i, c in enumerate(coeffs):
        m = mapping[i]['m']
        n = mapping[i]['n']
        mode_type = mapping[i]['type']
        # print(f"Order {m}, Mode {n}, Type {mode_type} has a coefficient of: {c}")

        if (mode_type == 'symmetric'):
            coeff_strength[int(m), int(n) - 1] = abs(coeffs[i])
        else:        # For cosine and sine pairs, calculate the magnitude
            if i % 2 == 0: continue
            coeff_cos = coeffs[i]
            coeff_sin = coeffs[i + 1]
            coeff_strength[int(m), int(n) - 1] = np.sqrt(coeff_cos**2 + coeff_sin**2)

    fig, ax = plt.subplots(figsize=(10, 8))
    im = ax.imshow(coeff_strength, aspect='auto', origin='lower', cmap='viridis')

    # Add labels and colorbar
    plt.colorbar(im, label='Coefficient Magnitude')
    ax.set_xlabel('Mode (n) - Number of Zeros')
    ax.set_ylabel('Order (m) - Number of Nodal Lines')
    ax.set_title('Bessel Mode Magnitude Map')

    # Set specific ticks for clarity
    ax.set_yticks(range(max_m + 1))
    ax.set_xticks(range(max_n), labels=[f'{i + 1}' for i in range(0, max_n)])

    plt.show()

plot_coefficients(mapping_lstsq, coeffs_lstsq, max_m, max_n)
plot_coefficients(mapping_ridge, coeffs_ridge, max_m, max_n)