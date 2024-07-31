import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import sys

def gen_heatmap(arr, image_name):
    arr = arr.reshape(100, 100)
    arr[arr == 0] = np.nan
    sns.heatmap(arr)
    plt.savefig(image_name)
    plt.clf()

argc = len(sys.argv)
argv = sys.argv

if argc != 2:
    print(f'usage: {argv[0]} FILE')
    exit(1)

file = argv[1]

data = np.loadtxt(file, dtype=np.float64, delimiter=' ')
print(f'shape = {data.shape}')

gen_heatmap(data[:, 3], 'l1_miss.png')
gen_heatmap(data[:, 4], 'clstr.png')
gen_heatmap(data[:, 5], 'updt.png')
gen_heatmap(data[:, 6], 'bbox.png')
gen_heatmap(data[:, 7], 'ist.png')