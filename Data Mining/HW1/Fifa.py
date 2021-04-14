import pandas
import numpy as np
import matplotlib.pyplot as plt
from pandas.core.frame import DataFrame
import seaborn as sns

data = pandas.read_csv('players.csv')

print("head and tail of data : ")
print(data.head())
print(data.tail())

print("----------")

columns = list(data.columns)

is_NaN = data. isnull()
row_has_NaN = is_NaN. any(axis=1)
missing_values = data[row_has_NaN]
print(missing_values)


# for index, row in data.iterrows():
#     if row.isnull().any():
#         miss = data.columns[row.isnull()].values
#         print("column: ",index, '---',str(miss))


print('------weights------')

print('mean :', data['Weight'].mean())
print('max :', data['Weight'].max())
print('min :', data['Weight'].min())

print("------players-------")

result = data.groupby('Nationality').Nationality.agg('count').to_frame('count').reset_index().sort_values('count',
                                               ascending=False)


print("Max values: \n",result.head(5),"\n------------------------------")
print("Min values: \n",result.tail(5),"\n------------------------------")
# mini = -1
# min_data = ""
# max_data = ""
# maxi = -1
# i = 0
# len = len(result)
# print("Max values: \n")
# for index,row in result.iterrows():
#     if i < 5:
#         print("Nation --> : "+str(row['Nationality']) +" --- count:  "+str(row['count']))
#     elif i == len-5:
#         print("\nMin values: ")
#     elif i > len-5:
#         print("Nation --> : "+str(row['Nationality']) +" --- count:  "+str(row['count']))

#     i+=1
#     if mini == -1 or row['count'] < mini:
#         mini = row['count']
#         min_data = "Nation --> : "+str(row['Nationality']) +" --- count:  "+str(mini)

#     if row['count'] > maxi:
#         maxi = row['count']
#         max_data = "Nation --> : "+str(row['Nationality']) +" --- count:  "+str(maxi)

# print("final : ")
# print("min : "+min_data)
# print("----")
# print("max: " +max_data)



print("-----Protential & Growth-----")
future_stars = data[(data['Growth'] > 4) & (data['Potential'] > 84)]
print(future_stars.head(5))
print(future_stars.tail(5))



print("-----plot-----")
calc = future_stars[['Potential', 'Positions']]
calc = calc.assign(Positions=calc["Positions"].str.split(","))
calc = calc.explode('Positions')
# for index,row in calc.iterrows():
#     t = row['Positions'].split(',')
#     row['Positions'] = t

sns.set_style("whitegrid")
# sns.scatterplot(data=calc, x='ID', y='Positions')
sns.boxenplot(data=calc,y='Potential',x='Positions')
plt.show()


print("-----clubs-----")
clubs = future_stars.groupby(['Club']).Club.agg('count').to_frame('count').reset_index().sort_values('count',
                                               ascending=False)

print('Best stars in future with Clubs in order: \n', clubs.head(5))


print("-----Chelsea-----")
print('Chelsea Stars in future : ',future_stars[future_stars['Club'] == 'Chelsea']['ValueEUR'].sum())



print("-----retirements-----")
print('players retirement numbers untill 2021 = ',data[(data['ContractUntil'] == 2021) & (data['NationalTeam'] == 'Not in team')]['ID'].count())


print("-----taromi data-----")

print(data[(data['FullName'] == 'Mehdi Taremi') & (data['ContractUntil'] > 2020)][['FullName', 'Positions', 'WageEUR', 'Club']])