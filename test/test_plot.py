# -----------------------------------------------------------------------------
#
#
# -----------------------------------------------------------------------------
import sys, os
import numpy as np
import matplotlib.pyplot as plt

# ...
inp = np.genfromtxt(sys.argv[1], delimiter=",")
outp = np.genfromtxt(sys.argv[2], delimiter=",")

# ...
dat = inp[:]

# ...
idx = outp[:,0]
real = outp[:,1]
imag = outp[:,2]
magn = outp[:,3]

# ...
plt.figure()
plt.subplot(4,1,1)
plt.plot(dat)
plt.title("Signal")
plt.subplot(4,1,2)
plt.plot(real)
plt.title("Real")
plt.subplot(4,1,3)
plt.plot(imag)
plt.title("Imaginary")
plt.subplot(4,1,4)
plt.plot(magn)
plt.title("Magnitude (Estimate)")

# ...
plt.tight_layout()
plt.show()
