import numpy as np
import sys
import pickle
files = open(sys.argv[1], 'r').read().splitlines()
print len(files)
print len(files[0].split())

components = int(sys.argv[3])

img_dim = len(files[0].split())
total_files = len(files)
matrix = np.zeros((img_dim, total_files))
#print matrix.shape
for f in range(0, len(files)):
    print f
    matrix[:, f] = files[f].split()

#print matrix[:, 0]
print matrix.shape

#covariance matrix

covariance = np.cov(matrix)
mode = str(sys.argv[2])
#eigen vectors
#eig_vals = open('pca_eig_values_4800_' + mode, 'wb')
#eig_vectors_900 = open('pca_4800_dim_3600_' + mode , 'wb')
#eig_vectors_600 = open('pca_4800_dim_2400_' + mode, 'wb')
#eig_vectors_300 = open('pca_4800_dim_1200_' + mode, 'wb')
eigen_values, eigen_vectors = np.linalg.eig(covariance)
#pickle.dump(eigen_values, eig_vals)
#eig_vals.close()
#pickle.dump(eigen_vectors[0:3600], eig_vectors_900)
#eig_vectors_900.close()
#pickle.dump(eigen_vectors[0:2400], eig_vectors_600)
#eig_vectors_600.close()
#pickle.dump(eigen_vectors[0:1200], eig_vectors_300)
#eig_vectors_300.close()

print eigen_vectors.shape
eigen_vectors = eigen_vectors[0:components]
print eigen_vectors.shape

pca_mat = np.dot(eigen_vectors.real, matrix)

print pca_mat.shape

pca_file = open('pca_' + str(components) + '_dim_' + mode, 'w')
pca_mat = pca_mat.T
for i in range(0, pca_mat.shape[0]):
    for j in range(0, pca_mat.shape[1]):
    	pca_file.write(str(pca_mat[i, j]))
    	pca_file.write(' ')
    pca_file.write('\n')

pca_file.close()
