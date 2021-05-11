import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import copy

def k_means(data, k, max_itr):
    min_valus = data.min()
    max_valus = data.max()

    random_centers = list()
    dimention = len(data.columns)
    clusters = list()
    points_group = []
    for i in range(len(data)):
        points_group.append(0)
    for i in range(k):
        new_random_center = []
        for j in range(dimention):
            new_random_center.append(np.random.uniform(low=min_valus[j], high=max_valus[j]))

        print(new_random_center)
        random_centers.append(new_random_center)
        clusters.append(list())


    itr = 0
    centers = random_centers
    data_list = data.values.tolist()

    while itr < max_itr:
        row_index = 0
        for i in clusters:
            i.clear()
        new_centers = copy.deepcopy(centers)
        for index,row_data in data.iterrows():
            row = row_data.values.tolist()
            min_index = dimention
            min_distance = -1
            for c in range(k):
                dist = np.sum(np.square(np.array(row) - np.array(centers[c])))

                if min_distance == -1 or min_distance >= dist:
                    min_distance = dist
                    min_index = c
            clusters[min_index].append(row)
            points_group[index] = min_index
        last_index =0
        #setting new centers values
        for i in range(k):
            if len(clusters[i]) == 0:
                continue
            cluster_points = pd.DataFrame(clusters[i])
            new_centers[i] = cluster_points.mean().values.tolist()
        # print("new centers : \n",new_centers)
        # print("old centers: \n",centers)

        if centers == new_centers:
            break
        centers = new_centers
        itr+=1
    for i in clusters:
        print(i)
    data['cluster'] = points_group
    print(data)
    return clusters


def error_calculator(data,k):
    errors = []
    for i in range(k):
        current_cluster = data.loc[data['cluster'] == i]

        center = current_cluster.mean()
        error = 0
        for index,point in current_cluster.iterrows():
            dist = np.sum(np.square(np.array(point) - np.array(center)))
            error += dist ** 0.5
        if len(current_cluster) != 0:
            error = error/len(current_cluster)
            errors.append(error)
    print(np.mean(errors))
    return np.mean(errors)


if __name__=='__main__':
    data = pd.read_csv('./datasets/iris.csv')
    data.drop('Species', inplace=True, axis=1)  # read only data points
    errors = []
    for k in range(1,6):
        data_copy = copy.deepcopy(data)
        k_means(data_copy, k, 50)
        errors.append(error_calculator(data_copy,k))
    plt.plot(list(range(1, 6)), errors, '-bo')
    ax = plt.gca()
    ax.set_xlabel('k')
    ax.set_ylabel('MAE')
    plt.show()