import numpy as np
import sys
print sys.argv[1]
print sys.argv[2]
files = open(sys.argv[1], 'r').read().splitlines()

print len(files)
print files[0].split()
class_number = 0
mode = sys.argv[2]
val = 0
if mode == 'train':
   val = 50
else:
   val = 10
svm_2 = open('pca_svm_'+ str(sys.argv[2]) + '_format', 'w')
for i in range(0, len(files)):
    if i % val == 0:
	class_number += 1
    svm_2.write(str(class_number))
    svm_2.write(' ')
    for j in range(0, len(files[i].split())):
	svm_2.write(str(j+1))
	svm_2.write(':')
	svm_2.write(str(files[i].split()[j]))
	svm_2.write(' ')
    svm_2.write('\n')

svm_2.close()



