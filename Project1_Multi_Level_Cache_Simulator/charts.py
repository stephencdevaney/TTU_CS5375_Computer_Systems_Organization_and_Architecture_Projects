import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

cache_dataset = pd.read_csv("cache_dataset1.csv")
fig, axes = plt.subplots(nrows = 2, ncols = 1, figsize=(10,10), sharex=True)
axes[0].plot(cache_dataset["way"], cache_dataset["trace.stream"], color="crimson")
axes[0].set_title("trace.stream trace file")
axes[1].plot(cache_dataset["way"], cache_dataset["trace.hpcg"], color="black")
axes[1].set_title("trace.hpcg trace file")
fig.suptitle('Way Comparison\n64B Block Size and 32KB Cache Size Cache Simulations')
fig.supxlabel('Associativity')
fig.supylabel('Hit Rate')
#plt.show()
plt.savefig("cache_dataset1.png")
plt.clf() # clear figure

cache_dataset = pd.read_csv("cache_dataset2.csv")
width = .4
bar1 = np.arange(len(cache_dataset["trace_files"]))
bar2 = [i+width for i in bar1]
plt.figure(figsize=(10, 10))
plt.bar(bar1, cache_dataset["Single_Level"], width, color="black", label="Single Level")
plt.bar(bar2, cache_dataset["Multiple_Level"], width, color="crimson", label="Multi Level")
plt.xticks(bar1+width/2, cache_dataset["trace_files"])
plt.title("Level Comparison\nCommon L1: 2-WAY 64B Block Size 64KB Cache Size\nAdditional L2: 8-WAY 64B Block Size 1MB Cache Size")
plt.xlabel("Trace Files")
plt.ylabel("Hit Rate")
plt.legend()
#plt.show()
plt.savefig("cache_dataset2.png")
plt.clf() # clear figure

