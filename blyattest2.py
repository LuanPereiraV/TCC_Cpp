import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import matplotlib
import time
# matplotlib.use('TKAgg')

n = 80

l = n/10

xmin = -l
xmax = l

ymin = -l
ymax = l

dt = 0.005

x = np.linspace(xmin, xmax, n)
y = np.linspace(ymin, ymax, n)

# print(x)

u = np.zeros((n, n))
v = np.zeros((n, n))

X, Y = np.meshgrid(x, y)

kx = 1.0*(2.0*np.pi)
ky = 0

disp = 3

u = np.cos(kx*(X) + ky*(Y)) * np.exp(-1/disp*(X - l//2)**2.0) * np.exp(-1/disp*(Y)**2.0)
v = -np.sin(kx*(X) + ky*(Y)) * np.exp(-1/disp*(X - l//2)**2.0) * np.exp(-1/disp*(Y)**2.0)

a = 2.0*l/(n+1)
sc = 1.0/np.max(np.sqrt(u**2+v**2))
u = sc*u
v = sc*v
sc = 0.5/(a**2)

print(sc)

# V =  Barreira potencial:
V = np.zeros((n, n))
V[:, n//2:(n//2)+1] = 30

V[30:36, :] = 0
V[44:50, :] = 0


grid = np.copy(u)

def iterate():
  global u, v, grid

  H = np.zeros(u.shape)

  for i in range(1, n-1):
    for j in range(1, n-1):
      H[i][j] = (u[i+1][j] + u[i-1][j] + u[i][j+1] + u[i][j-1]) - 4.0*u[i][j]


  H = -sc * H

  H = H + V * u
  v = v - H * dt

  for i in range(1, n-1):
    for j in range(1, n-1):
      H[i][j] = (v[i+1][j] + v[i-1][j] + v[i][j+1] + v[i][j-1]) - 4.0*v[i][j]

  H = -sc * H

  H = H + V * v
  u = u + H * dt

  grid = np.sqrt(u**2.0 + v**2.0)
  # grid = u
  # grid = u
  # print(np.max(H))
  # grid = (np.abs(u) + np.abs(v))


def update(*args):
  iterate()

  im.set_data((grid + V))

  return [im]

fig, ax = plt.subplots()

im = ax.matshow((grid + V), vmin=0, vmax=1, animated=True, cmap="inferno")
fig.colorbar(im)

anim = FuncAnimation(fig, update, frames=360, interval=10, blit=True)

# anim.save('simu.mp4', fps=15, extra_args=['-vcodec', 'libx264'])

print("Pronto!")

plt.show()
