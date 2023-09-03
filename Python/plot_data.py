import matplotlib.pyplot as plt
import pandas as pd


episode_data = pd.read_csv('EpisodeData.csv')

for column in ['Reward', 'Epsilon', 'Loss']:
    plt.plot(episode_data['Episode'], episode_data[column])
    plt.ylabel(column)
    plt.savefig(f'{column}Plot.png')
    plt.clf()
