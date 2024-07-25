import pyvista as pv
import numpy as np
import sys

argc = len(sys.argv)
argv = sys.argv

if argc != 7:
    print('usage: python vis_ray.py MODEL_FILE RAY_FILE RESULT_FILE MAX_T NUM_RAYS RANDOMIZE')
    exit(1)

model_file = argv[1]
ray_file = argv[2]
result_file = argv[3]
max_t = float(argv[4])
num_rays = int(argv[5])
randomize = int(argv[6])

p = pv.Plotter()
ply_mesh = pv.read(model_file)
p.add_mesh(ply_mesh, opacity=0.1)

rs = np.fromfile(ray_file, dtype=np.float32).reshape(-1, 7)
result = np.fromfile(result_file, dtype=np.float32).reshape(-1, 2)
assert rs.shape[0] == result.shape[0]

if randomize:
    indices = np.arange(rs.shape[0])
    np.random.shuffle(indices)
    indices = indices[:num_rays]
else:
    indices = np.arange(num_rays)

rs = rs[indices]
result = result[indices]

for i in range(num_rays):
    ls = np.array([rs[i][0], rs[i][1], rs[i][2]])
    if result[i][0] == 0:
        multiplier = max_t
        color = 'g'
    else:
        multiplier = result[i][1]
        color = 'r'
    
    le = ls + multiplier * np.array([rs[i][3], rs[i][4], rs[i][5]])
    ray = pv.Line(ls, le)
    p.add_mesh(ray, color=color)

p.show()