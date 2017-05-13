
"""
http://stackoverflow.com/questions/2148543/how-to-write-a-confusion-matrix-in-python
"""
import numpy as np
import pandas as pd
import sys
import os
'''
names = os.listdir("train/")
na = dict()
for i in range(0, len(names)):
    na[str(i+1)] = names[i]
print na
'''
targets = []
curr = 0
for i in range(0, 25):
    curr +=1
    for j in range(0, 10):
	targets.append(str(curr))
#print targets
filename = open(sys.argv[-1], 'r').read().splitlines()
pred = []

#print filename[0]

#print filename[1]
for i in range(len(filename)):
    #print filename[i][0:2]
    pred.append(filename[i][0:2])

targets = np.array(targets).astype(int)
pred = np.array(pred).astype(int)

from collections import defaultdict

def confusion_matrix(expected, predicted):
    expected = np.array(expected, dtype = int)
    predicted = np.array(predicted, dtype = int)
    f = np.arange(1, 26, 1)
    new_dict = defaultdict(dict)
    for fe in f:
        for fee in f:
            new_dict[fe][fee] = np.sum((predicted == fe) & (expected ==fee))
    return new_dict

x = confusion_matrix(targets, pred)
new_y = []
y = np.array(np.arange(0, 26, 1)).tolist()
for ye in y:
    new_y.append(str(ye).zfill(1))
print 'Confusion Matrix Nearest Neighbors: '
print new_y
for xe in x.keys():
    nl = []
    nl.append(str(xe).zfill(2))
    for xee in x[xe].keys():
        nl.append(str(x[xe][xee]).zfill(1))
    print nl
'''
new_targets = []
new_pred = []
for i in range(0, len(targets)):
    new_targets.append(na[targets[i]])
    new_pred.append(na[pred[i]])

a = pd.Series(targets, name = 'Actual')
b = pd.Series(pred, name = 'Pred')
conf = pd.crosstab(a, b)
print conf

print "Accruacy = " + str(np.sum(targets == pred)/250. )
'''



#print targets
#print pred
#targets = np.reshape(np.array(targets), (250, 1))
#pred = np.reshape(np.array(pred), (250, 1))

#print targets
#print pred
#print np.sum(targets == pred)
#print targets.shape
#print pred.shape
