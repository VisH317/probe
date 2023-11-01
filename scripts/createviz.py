import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

df = pd.read_csv("../build/updates.csv")

df = df[df["layernum"]==1]

sns.set_theme()

plt.title("Hidden layer Jitter Algorithm Distortions")

plt.xlabel("neuron ID (50 total neurons)")
plt.ylabel("average weight distortion")

plt.bar(x=df["neuronid"]-100, height=df["distortion"])

plt.savefig("fig.png")
