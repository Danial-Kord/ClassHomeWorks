'''
    code from here is used: "https://scikit-learn.org/stable/auto_examples/cluster/plot_dbscan.html"
'''
import numpy as np
import pandas as pd
from sklearn.cluster import DBSCAN
import matplotlib.pyplot as plt
from p1 import k_means

if __name__=='__main__':
    data = pd.read_csv('./datasets/worms.csv',index_col=0)

    plt.scatter(data['X'], data['Y'], s=.005)
    plt.show()

    eps_and_min_samples_list = [[36,17],[35,16],[32,12],[28,12],[32,16],[25,15],[30,20],[35,20],[35,15],[15,10],[20,15],[20,20]]
    for test in eps_and_min_samples_list:

        min_samples = test[0]
        eps = test[1]
        db = DBSCAN(eps=eps, min_samples=min_samples).fit(data.to_numpy())


        labels = db.labels_


        unique_labels = set(labels)
        colors = [plt.cm.Spectral(each)
                  for each in np.linspace(0, 1, len(unique_labels))]
        for k, col in zip(unique_labels, colors):
            if k == -1:
                col = [0, 0, 0, 0.001]
            class_member_mask = (labels == k)

            xy = data[class_member_mask]
            plt.title("eps="+str(eps) +" and min samples= "+str(min_samples)+" clusters= "+str(len(unique_labels)))
            plt.scatter(xy['X'], xy['Y'], c=[col], s=.005)

        plt.show()
